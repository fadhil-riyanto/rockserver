#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "../header/epoll_watcher.h"
#include "config.c"
#include "../submodule/log.c-patched/src/log.h"
#include "../header/threading.h"


static void setsignal_thread_free(server_state_t *server_state, 
                                            int thread_num)
{
        log_debug("thread ended %d", thread_num);
        struct threading_ctx* structdata = (struct threading_ctx*)server_state->threading_ch_ctx;
        structdata[thread_num].state = 0;
        structdata[thread_num].need_join = 1;
}

void handle_conn(int clientfd, server_state_t *server_state, int thread_num) {

        log_debug("thread %d", thread_num);
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
            log_info("handled fd %d", clientfd);

            epoll_watch(clientfd, child_epfd, &child_event);

            while (1) {
                child_epfd_event_len = epoll_wait(child_epfd, child_events, CHILD_MAXEVENTS, 1000);
                for(int i = 0; i < child_epfd_event_len; i++) {
                        ret = recv(child_events[i].data.fd, data, length, 0);
                        if (ret == 0 || ret == -1) {
                                goto thread_ask_to_exit;
                        } else {
                                data[ret] = '\0';
                                log_debug("fd %d on thread %d says: %s", child_events[i].data.fd, thread_num, 
                                                        data);
                                if (strcmp(data, "exit\n") == 0) {
                                    goto thread_ask_to_exit;
                                }
                        }
                    }

                    if (*server_state->exit_now == 1) {
                        thread_ask_to_exit:
                        log_info("thread exit");
                        break;
                    }
            }

        }
        
        setsignal_thread_free(server_state, thread_num);

        free(data);
        close(child_epfd);
}