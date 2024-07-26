
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXBUF 65535
#define OPCODE_LEN 3

void get_opcode(char* target, char *src)
{
        for(int i = 0; i < 3; i++) {
                *(target + i) = *(src + i);
        }
}

void get_value(char* target, char *src, int n)
{
        // printf("%s\n", src);
        for(int i = OPCODE_LEN; i < OPCODE_LEN + n; i++) {
                *(target + i - OPCODE_LEN) = *(src + i);
                
        }
}

int main() 
{
        char *opcode = (char*)malloc(sizeof(char) * 3);
        char *value = (char*)malloc(sizeof(char) * MAXBUF);

        char* sample = "setabcASDDDDDDDDDDDDDDDDDDDDSDASDASAAAaaa";

        get_opcode(opcode, sample);
        get_value(value, sample, strlen(sample));

        printf("%s\n", opcode);
        printf("%s\n", value);

        free(opcode);
        free(value);
}