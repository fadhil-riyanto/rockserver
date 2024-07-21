#define USE_DEBUG_FN

#include "string.h"
#include <stdlib.h>
#include <string.h>
#include "debugutils.h"

/* test/left_string.cc */
void left_string(char* targetstr, int origin_length)
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


/* test file: test/mv_string.cc */
void first_strnmv(char *src, char *dest, int n)
{       
        // char *dest = (char*)malloc(sizeof(char) * (n + 1));
        // memset(dest, 0, sizeof(char) * (n + 1));

        int i = 0;
        for(; i < (n + 1); i++) {
                *(dest + i) = *(src + i);
                *(src + i) = '\0';
        }
        *(dest + i) = '\0';

        // return dest;
        // idd(i);
}


/* test file: test/splitstring.cc */
int find_first_text_off(const char* data, int len)
{
        static const char DELIMITER[4] = {
                '\r', '\n', '\r', '\n'
        };

        for(int i = 0; i < len; i++) {
                if (*(data + i + 1) == DELIMITER[0] && *(data + i + 2) == DELIMITER[1] &&
                        *(data + i + 3) == DELIMITER[2] && *(data + i + 4) == DELIMITER[3]) 
                {
                        return i;
                }
                
        }
        return -1;
}

/* test/remove_string.cc */

void zerostr(char* src, int start, int length)
{
        length = start + length;
        for(int i = 0; i <= (length - 1); i++) {
                // idd(i);
                if (i >= start && i <= (start + length)) {
                        *(src + i) = '\0';
                        // idd(i);
                }
        }
}