#ifndef ROCKSDB_TCP_PARSER
#define ROCKSDB_TCP_PARSER

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
        int cur_multiplication1;
        int cur_multiplication2;
} __attribute__((__aligned__(32))) ; /* reserved first 32 bit align */

void alloc_parse(struct parse_res *res);
void free_parse(struct parse_res *res);
int parse(char* text, int len_alloc, struct parse_res *res);

#endif