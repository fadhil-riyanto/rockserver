#include "header/threading.h"
#include "config.c"

void init_thread(struct threading_ctx *th) {
    for(int i = 0; i < MAX_CONN; i++) {
        th[i].state = 0;
        th[i].th_index = i;
    }
}

int get_free_thread(struct threading_ctx *th) {
    for(int i = 0; i < MAX_CONN; i++) {
        if (th[i].state == 0) {
            return th[i].th_index;
        }
    }
}