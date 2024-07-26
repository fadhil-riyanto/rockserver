#include <stdio.h>
#include <string.h>

struct rocksdb_protocol {
        long len;

        int op1len;
        int op2len;

        char* op1;
        char* op2;
};

static void read_data(struct rocksdb_protocol *data, size_t structsz)
{
        unsigned char buffer[30];

        FILE *fdptr;
        fdptr = fopen("shared_data", "rb");

        if (fdptr == NULL) 
                perror("fopen");

        fread(data, sizeof(buffer), 1, fdptr);
        // data = (struct rocksdb_protocol*)buffer;
}

int main()
{
        struct rocksdb_protocol payload;

        read_data(&payload, sizeof(payload));
        printf("%d\n", payload.op2len);
}