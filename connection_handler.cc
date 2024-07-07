#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "header/connection_handler.h"
#include <signal.h>
#include "header/epoll_watcher.h"
#include "config.c"
#include "submodule/log.c-patched/src/log.h"

void handle_conn(int clientfd, volatile sig_atomic_t *signal_exit_now) {

    size_t length = sizeof(char) * 65535;
    int ret;
    int child_epfd;
    int child_epfd_event_len;
    struct epoll_event child_event, child_events[CHILD_MAXEVENTS];

    epoll_init(&child_epfd);

    char *data = (char*)malloc(length);
    if (data == NULL) {
        perror("malloc failed");
    } else {
        memset(data, 0, length);
        printf("handled fd %d\n", clientfd);

        epoll_watch(clientfd, child_epfd, &child_event);

        while (1) {
            child_epfd_event_len = epoll_wait(child_epfd, child_events, CHILD_MAXEVENTS, 1000);
            for(int i = 0; i < child_epfd_event_len; i++) {
                ret = recv(child_events[i].data.fd, data, length, 0);
                if (ret == 0 || ret == -1) {
                    break;
                } else {
                    data[ret] = '\0';
                    log_debug("fd %d says: %s", child_events[i].data.fd, data);
                    if (strcmp(data, "exit\n") == 0) {
                        break;
                    }
                }
            }

            if (*signal_exit_now == 1) {
                log_info("thread exit");
                break;
            }

            
            // close(clientfd);
          
            
        }

    }
    
    free(data);
    close(clientfd);
}