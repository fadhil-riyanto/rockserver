#ifndef THREADING_H
#define THREADING_H

#include <thread>

struct threading_ctx {
    std::thread handler;
    int state;
    int th_index;
};

void init_thread(struct threading_ctx *th);
int get_free_thread(struct threading_ctx *th);
void fill_thread(struct threading_ctx *th, int thnum, void (*f)());
int clean_thread(struct threading_ctx *th);

#endif