#include "parse.c"

int main()
{
        char setcommand[] = "set \"ajsjdhsjdj\" \"aaaaaaaaaa\"";

        struct parse_res res;
        alloc_parse(&res);
        parse(setcommand, strlen(setcommand), &res);

        printf("%s\n", res.op2);
        free_parse(&res);
}