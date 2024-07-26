#include <stdio.h>
#include <string.h>

struct rocksdb_protocol {
        long len;

        int op1len;
        int op2len;

        char op1[20];
        char op2[65535];
};

static void write_data(struct rocksdb_protocol *data, size_t structsz)
{
        FILE *fdptr;
        fdptr = fopen("shared_data", "wb");

        if (fdptr == NULL) 
                perror("fopen");

        fwrite(data, structsz, 1, fdptr);
}

int main()
{
        

        char* key = "asdfssss";
        char* value = "a";

        struct rocksdb_protocol payload[1] = {
                {
                        (sizeof(int) * 2) + (strlen(key) + strlen(value)),
                        strlen(key),
                        strlen(value)
                }
                
        };

        write_data(&payload, sizeof(payload));
        // printf("%lu\n", );
}