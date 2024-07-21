

#ifdef USE_DEBUG_FN

#include <stdio.h>

#define sdd(x) printf("debug string: \"%s\"\n", x);
#define cdd(x) printf("debug char: \"%c\"\n", x);
#define idd(x) printf("debug int: \"%d\"\n", x);

#define __debug_str(target, approx)                                                     \
{                                                                               \
        /* test len is 10 */                                                    \
                                                                                \
        for(int i = 0; i < approx; i++) {                                           \
                if (*(target + i) == '\0') {                                    \
                        printf("%c", '0');                                      \
                } else if (*(target + i) == '\r') {                             \
                        printf("%c%c", '\\', 'r');                              \
                } else if (*(target + i) == '\n') {                             \
                        printf("%c%c", '\\', 'n');                              \
                } else {                                                        \
                        printf("%c", *(target + i));                            \
                }                                                               \
        }                                                                       \
        printf("\n");                                                           \
}                                                                               \

#endif