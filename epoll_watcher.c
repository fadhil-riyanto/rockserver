#include "header/epoll_watcher.h"

int epoll_init(int *epfd) {
    *epfd = epoll_create1(0);
    if (*epfd == -1) {
        fprintf(stderr, "epoll init failed");
        return -1;
    }
}
