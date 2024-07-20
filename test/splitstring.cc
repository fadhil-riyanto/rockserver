#include <string.h>
#include <gtest/gtest.h>


#define USE_DEBUG_FN 1

#ifdef USE_DEBUG_FN
#define sdd(x) printf("debug string: \"%s\"\n", x);
#define cdd(x) printf("debug char: \"%c\"\n", x);
#define idd(x) printf("debug int: \"%d\"\n", x);
#endif 

/* goal:
        split string by \r\n\r\n, find index of 1, return the offset of \r\n first

        example: abc\r\n\r\nacve\r\n\r\n
        return: 2 (text)
                3 (separator) + 4 
*/

static const char DELIMITER[4] = {
        '\r', '\n', '\r', '\n'
};

static const char sample[20] = "abcdedefghiasdf";

static int find_first_text_off(const char* data, int len)
{
        for(int i = 0; i < len; i++) {
                if (*(data + i + 1) == DELIMITER[0] && *(data + i + 2) == DELIMITER[1] &&
                        *(data + i + 3) == DELIMITER[2] && *(data + i + 4) == DELIMITER[3]) 
                {
                        return i;
                }
                
        }
        return -1;
}

int main() 
{
        int foundoff = find_first_text_off(sample, strlen(sample));
        idd(foundoff);
}