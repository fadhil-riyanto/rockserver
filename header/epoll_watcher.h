#ifndef EPOLL_WATCHER_H
#define EPOLL_WATCHER_H

#include <sys/epoll.h>

int epoll_init(int *epfd);
int epoll_watch(int fd, int epfd, struct epoll_event *event);
void epoll_cleanup(int *epfd);

#endif