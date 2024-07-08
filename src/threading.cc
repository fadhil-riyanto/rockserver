#include "../header/threading.h"
#include "../header/server.h"
#include "config.c"
#include <thread>
#include <unistd.h>
#include "../submodule/log.c-patched/src/log.h"

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

void fill_thread(struct threading_ctx *th, int thnum, void (*f)(int, server_state_t*), int acceptfd, server_state_t *server_state) {
    th[thnum].handler = std::thread(f, acceptfd, server_state);
    th[thnum].state = 1;
    th[thnum].th_index = thnum;
    th[thnum].acceptfd_handler = acceptfd;
}

int clean_thread(struct threading_ctx *th) {
    int ret;
    for(int i = 0; i < MAX_CONN; i++) {
        if (th[i].state == 1) {
            ret = close(th[i].acceptfd_handler);
            log_debug("closing thread %d with status %d", i, ret);
            th[i].handler.join();
        }
        
    }
    return -1;
}