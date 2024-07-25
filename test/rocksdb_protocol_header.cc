struct protocol_template {
        long content_length;

        short opcode;
        int op1_length;
        int op2_length;
        char *op1;
        char *op2;
}  __attribute__((__aligned__(64)));

/* test write to file */

int main()
{
        struct protocol_template protocol = {
                .content_length = 3
        }
}