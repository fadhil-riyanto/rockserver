#include <stdlib.h>
#include <stdio.h>

#define USE_DEBUG_FN 1

#ifdef USE_DEBUG_FN
#define sdd(x) printf("debug string: \"%s\"\n", x);
#define cdd(x) printf("debug char: \"%c\"\n", x);
#define idd(x) printf("debug int: \"%d\"\n", x);
#endif

static char src[10] = "abcdefhin";


static char* first_strnmv(char *src, int n)
{       
        char *dest = (char*)malloc(sizeof(char) * (n + 1));

        int i = 0;
        for(; i < (n + 1); i++) {
                *(dest + i) = *(src + i);
                *(src + i) = '\0';
        }
        *(dest + i) = '\0';

        return dest;
        // idd(i);
}

static void __debug_str(char* target)
{
        /* test len is 10 */

        for(int i = 0; i < 10; i++) {
                printf("%c", *(target + i) == '\0' ? '0' : *(target + i));
        }
        printf("\n");
}

int main() 
{
        __debug_str((char*)&src[0]);
        char* dest = first_strnmv(src, 4);
        
        __debug_str((char*)&src[0]);
        __debug_str(dest);

        free(dest);
}