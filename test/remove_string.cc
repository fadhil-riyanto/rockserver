#define USE_DEBUG_FN
#include "debug.h"



void zerostr(char* src, int start, int length)
{
        for(int i = 0; i <= (length - 1); i++) {
                if (i >= start && i <= (start + length)) {
                        *(src + i) = '\0';
                }
                idd(i);
        }
}

int main()
{
        char sample[30] = {
                '\0', '\0', '\0', '\0', '\0', '\0', 
                '\r', '\n', '\r', '\n', 'j', 'k', 
                '\0', '\0', '\0', '\0', '\0', '\0', 
                '\0', '\0', '\0', '\0', '\0', '\0', 
                '\0', '\0', '\0', '\0', '\0', '\0', 
        };

        rmstr(sample, 6, 6 + 4);

        __debug_str(sample, 30);
}