#include "../header/epoll_watcher.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>

int epoll_init(int *epfd)
{
        *epfd = epoll_create1(0);
        if (*epfd == -1) {
                fprintf(stderr, "epoll init failed");
                return -1;
        } 
        return 0;
}

int epoll_watch(int fd, int epfd, struct epoll_event *event)
{
        event->events = EPOLLIN;
        event->data.fd = fd;

        if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, event) == -1) {
                fprintf(stderr, "error add epoll fd");
                return -1;
        }
        return 0;
}

void epoll_cleanup(int *epfd)
{
        close(*epfd);
}