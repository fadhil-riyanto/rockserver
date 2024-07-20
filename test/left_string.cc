#include <stdio.h>

#define USE_DEBUG_FN 1
#ifdef USE_DEBUG_FN
#define sdd(x) printf("debug string: \"%s\"\n", x);
#define cdd(x) printf("debug char: \"%c\"\n", x);
#define idd(x) printf("debug int: \"%d\"\n", x);
#endif

static void left_string(char* targetstr, int origin_length)
{
        char temp = 0;
        int c = 0;
        do {
                temp = *(targetstr + origin_length + c);
                /* rewrite memory */

                *(targetstr + origin_length + c) = '\0';

                /* move value to the first, and next */
                *(targetstr + c) = temp;
                c++;
        } while (temp != '\0');

        // sdd(targetstr);
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
        char str[10] = {
                '\0', '\0', 'h', 'e', 'y', 
                '\0', '\0', '\0', '\0', '\0'
        };

        // printf("%s\n", str);
        __debug_str(str);

        left_string(str, 0);

        __debug_str(str);


}