#include "../header/threading.h"
#include "../header/server.h"
#include "config.c"
#include <atomic>
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
        if (th[i].state == 0 && th[i].need_join == 0) {
            return th[i].th_index;
        }
    }
    return -1;
}

void fill_thread(struct threading_ctx *th, int thnum, 
                    void (*f)(int, server_state_t*, int), int acceptfd, 
                    server_state_t *server_state, int thread_num)
{
    th[thnum].handler = std::thread(f, acceptfd, server_state, thread_num);
    th[thnum].state = 1;
    // th[thnum].need_clean.store(0, std::memory_order::memory_order_seq_cst);
    th[thnum].need_join = 0;
    th[thnum].th_index = thnum;
    th[thnum].acceptfd_handler = acceptfd;
}

void clean_thread_queue(struct threading_ctx *th)
{
    for(int i = 0; i < MAX_CONN; i++) {
        if (th[i].state == 0 && th[i].need_join == 1) {
            log_debug("cleaning unused thread #%d", i);
            close(th[i].acceptfd_handler);
            th[i].handler.join();
            th[i].need_join = 0;
        }
    }
        
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