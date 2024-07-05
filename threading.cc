#include "header/threading.h"
#include "config.c"
#include <thread>

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
    return -1;
}

void fill_thread(struct threading_ctx *th, int thnum, void (*f)(int), int acceptfd) {
    th[thnum].handler = std::thread(f, acceptfd);
    th[thnum].state = 1;
    th[thnum].th_index = thnum;
    th[thnum].acceptfd_handler = acceptfd;
}

int clean_thread(struct threading_ctx *th) {
    for(int i = 0; i < MAX_CONN; i++) {
        th[i].handler.join();
    }
    return -1;
}