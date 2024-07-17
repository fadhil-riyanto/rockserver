
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define USE_DEBUG_FN 1

#ifdef USE_DEBUG_FN
#define sdd(x) printf("debug: \"%s\"\n", x);
#define cdd(x) printf("debug: \"%c\"\n", x);
#define idd(x) printf("debug: \"%d\"\n", x);
#endif

#define MAX_PAGESIZE (getpagesize() / 2)
#define ARR_LEN(x) (sizeof(x) / sizeof(char))

#define OPCODE_LENGTH 3
#define __hot  __attribute__((hot))

enum rockserver_op {
        RCK_COMMAND_SET,
        RCK_COMMAND_GET
};

struct parse_res 
{
        enum rockserver_op op_code;
        char *op1;
        char *op2;
        char *tmp_opcode;
} __attribute__((__aligned__(32))) ; /* reserved first 32 bit align */

static void alloc_parse(struct parse_res *res)
{
        // alloc first
        res->op1 = malloc(MAX_PAGESIZE);
        res->op2 = malloc(MAX_PAGESIZE);
        res->tmp_opcode = malloc(sizeof(char) * (OPCODE_LENGTH + 1));
        
}


static void free_parse(struct parse_res *res)
{
        // alloc first
        free(res->op1);
        free(res->op2);
        free(res->tmp_opcode);
        
}

static inline __hot void __find_opcode(char* text, struct parse_res *res) 
{
        int i = 0;
        
        for(;i < 3; i ++) {
                *(res->tmp_opcode + i) = *(text + i);
        }
        *(res->tmp_opcode + (i + 1)) = *(text + i);
}

static int __define_opcode(char *op_code_str, struct parse_res *res)
{
        if (strncmp(op_code_str, "set", OPCODE_LENGTH) == 0) {
                res->op_code = RCK_COMMAND_SET;
        } else if (strncmp(op_code_str, "get", OPCODE_LENGTH) == 0) {
                res->op_code = RCK_COMMAND_GET;
        } else {
                return -1;
        }
        return 0;
}

/* how parse works:
1. split '"' into 4 segment
2. - 0 segmen no counted
   = 1 segment counted as key
   - 2 segment counted as space
   - 3 segment counted as value
   - 4 segment counted as end

3. variable turn mean, its op1 turn or op2 turn
4. c1 and c2 is counter


*/
 
static int parse(char* text, int len_alloc, struct parse_res *res)
{
        int ret;
        int op1_req_len, op2_req_len;
        int turn = 0;
        int quote = 0;
        int c1 = 0;
        int c2 = 0;

        /* find the opcode */
        __find_opcode(text, res);
        ret = __define_opcode(res->tmp_opcode, res);
        if (ret < 0) {
                fprintf(stderr, "opcode invalid");
        }

        /* start from offset 3 */
        for(int i = 0; i < len_alloc; i++) {
                char tmp = *(text + i);
                if (tmp == '"') {
                        /* pass */
                        quote++;
                        if (quote == 2) {
                                turn = 1;
                        }
                } else {
                        if (quote == 1 && tmp != ' ' && turn == 0) {
                                *(res->op1 + c1) = tmp;
                                c1++;
                        }
                        if (quote == 3 && tmp != ' ' && turn == 1) {
                                *(res->op2 + c2) = tmp;
                                c2++;
                        }

                        if (quote == 2 && tmp == ' ') {
                                *(res->op1 + c1) = '\0';
                        }
                        if (quote == 4 && tmp == ' ') {
                                *(res->op2 + c2) = '\0';
                        }
                        // sdd("space");
                        
                }
        }

        sdd(res->op1);
        sdd(res->op2);


        return 0;
}

int main()
{
        char *setcommand = "set \"testbigvalueisset\" \"nowisvalue\"";

        struct parse_res res;
        alloc_parse(&res);
        parse(setcommand, strlen(setcommand), &res);

        // sdd(res.op_code);
        free_parse(&res);
        return 0;
}