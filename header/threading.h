#ifndef THREADING_H
#define THREADING_H

#include <thread>

struct threading_ctx {
    std::thread handler;
    int state;
    int th_index;
};

#endif