/* from test/parse.c */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "rocksdb_tcp_parser.h"

#define USE_DEBUG_FN

#ifdef USE_DEBUG_FN
#define sdd(x) printf("debug string: \"%s\"\n", x);
#define cdd(x) printf("debug char: \"%c\"\n", x);
#define idd(x) printf("debug int: \"%d\"\n", x);
#endif

#define MAX_PAGESIZE (getpagesize())
#define ARR_LEN(x) (sizeof(x) / sizeof(char))

#define OPCODE_LENGTH 3
#define __hot  __attribute__((hot))

void __pdebug_get_cur_pgsize()
{
        idd(MAX_PAGESIZE);
}

void alloc_parse(struct parse_res *res)
{
        // alloc first
        res->cur_multiplication1 = 1;
        res->cur_multiplication2 = 1;

        res->op1 = (char*)malloc(MAX_PAGESIZE * res->cur_multiplication1);
        res->op2 = (char*)malloc(MAX_PAGESIZE * res->cur_multiplication2);
        res->tmp_opcode = (char*)malloc( sizeof(char) * (OPCODE_LENGTH + 1));

        memset(res->op1, 0, sizeof(MAX_PAGESIZE * res->cur_multiplication1));
        memset(res->op2, 0, sizeof(MAX_PAGESIZE * res->cur_multiplication2));
        
}


void fixed_alloc_parse(struct parse_res *res, int op1size, int op2size)
{
        res->op1 = (char*)malloc(sizeof(char) * (op1size + 5));
        memset(res->op1, 0, sizeof(char) * (op1size + 5));

        res->op2 = (char*)malloc(sizeof(char) * (op2size + 5));
        memset(res->op2, 0, sizeof(char) * (op2size + 5));

        res->tmp_opcode = (char*)malloc( sizeof(char) * (OPCODE_LENGTH + 1));
}


int fixed_alloc_parse_packed(int op1size, int op2size)
{
        int all_len = (op1size + 1) + (op2size + 1) + (OPCODE_LENGTH + 1);
        return all_len;
}


void free_parse(struct parse_res *res)
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
        // *(res->tmp_opcode + (i + 1)) = *(text + i);
}

static int __define_opcode(char *op_code_str, struct parse_res *res)
{
        if (strncmp(op_code_str, "set", OPCODE_LENGTH) == 0) {
                res->op_code = RCK_COMMAND_SET;
        } else if (strncmp(op_code_str, "get", OPCODE_LENGTH) == 0) {
                res->op_code = RCK_COMMAND_GET;
        } else if (strncmp(op_code_str, "del", OPCODE_LENGTH) == 0) {
                res->op_code = RCK_COMMAND_DEL;
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
 
int parse(char* text, int len_alloc, struct parse_res *res)
{
        int ret;
        int op1_req_len = 0;
        int op2_req_len = 0;
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
                                op1_req_len++;
                        }
                        if (quote == 3 && tmp != ' ' && turn == 1) {
                                *(res->op2 + c2) = tmp;
                                c2++;
                                op2_req_len++;
                        }
                        // idd(res->cur_multiplication1);
                        /* expand memory */
                        if (op1_req_len > ((MAX_PAGESIZE * res->cur_multiplication1) - 1)) {
                                res->cur_multiplication1 = res->cur_multiplication1 + 1;
                                
                                res->op1 = (char*)realloc(res->op1, MAX_PAGESIZE * res->cur_multiplication1);
                        }

                        if (op2_req_len > ((MAX_PAGESIZE * res->cur_multiplication2) - 1)) {
                                res->cur_multiplication2++;
                                res->op2 = (char*)realloc(res->op2, MAX_PAGESIZE * res->cur_multiplication2);
                        }

                        // idd(op2_req_len);

                        if (quote == 2) {
                                *(res->op1 + c1) = '\0';
                        }
                        if (quote == 4) {
                                *(res->op2 + c2) = '\0';
                        }
                        // sdd("space");
                        
                }
        }
        return 0;
}