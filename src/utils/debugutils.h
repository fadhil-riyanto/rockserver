

#ifdef USE_DEBUG_FN

#include <stdio.h>

#ifdef USE_DEBUG_PRINT_LN
#define sddln(x) printf("debug %d string: \"%s\"\n", __LINE__, x);
#define cddln(x) printf("debug %d char: \"%c\"\n", __LINE__, x);
#define iddln(x) printf("debug %d int: \"%d\"\n", __LINE__, x);
#else
#define sddln(x)
#define cddln(x)
#define iddln(x)
#endif

#ifdef USE_DEBUG_PRINT
#define sdd(x) printf("debug string: \"%s\"\n", x);
#define cdd(x) printf("debug char: \"%c\"\n", x);
#define idd(x) printf("debug int: \"%d\"\n", x);
#else
#define sdd(x)
#define cdd(x)
#define idd(x)
#endif

#define __debug_str(target, approx)                                             \
{                                                                               \
        /* test len is 10 */                                                    \
                                                                                \
        printf("[DEBUGSTR]");                                                   \
        for(int i = 0; i < approx; i++) {                                       \
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
}            


#define __debug_str_ln(target, approx)                                             \
{                                                                               \
        /* test len is 10 */                                                    \
                                                                                \
        printf("[DEBUGSTR_%d]", __LINE__);                                                   \
        for(int i = 0; i < approx; i++) {                                       \
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