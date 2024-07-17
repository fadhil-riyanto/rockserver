
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
 
static int parse(char* text, int len_alloc, struct parse_res *res)
{
        int ret;
        int op1_req_len, op2_req_len;

        /* find the opcode */
        __find_opcode(text, res);
        ret = __define_opcode(res->tmp_opcode, res);
        if (ret < 0) {
                fprintf(stderr, "opcode invalid");
        }

        // sdd(res->tmp_opcode);
        idd(res->op_code);

        
        

        // for(int i = 0; i < len_alloc; i++) {
        //         cdd(*(text + i));
        //         if ((*text + 1) == 'a') {
        //                 printf("found double mark at off %d\n", i);
        //         }
        // }


        return 0;
}

int main()
{
        char *setcommand = "get \"aaaa\" \"bbbbb\"";

        struct parse_res res;
        alloc_parse(&res);
        parse(setcommand, strlen(setcommand), &res);

        // sdd(res.op_code);
        free_parse(&res);
        return 0;
}