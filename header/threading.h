#ifndef THREADING_H
#define THREADING_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

struct threading_ctx {
    std::thread handler;

    // client section
    struct sockaddr_in clientaddr;
    int acceptfd_handler;

    int state;
    int need_join;
    int th_index;
};

void init_thread(struct threading_ctx *th);
int get_free_thread(struct threading_ctx *th);
void fill_thread(struct threading_ctx *th, int thnum, void (*f)(int), int acceptfd);
int clean_thread(struct threading_ctx *th);

#endif