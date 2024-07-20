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
#include "utils/string.h"
#include "utils/debugutils.h"

#define char_length 65535
#define length (sizeof(char) * char_length)

static void setsignal_thread_free(server_state_t *server_state, 
                                            int thread_num)
{
        log_debug("thread ended %d", thread_num);
        struct threading_ctx* structdata = (struct threading_ctx*)server_state->threading_ch_ctx;
        structdata[thread_num].state = 0;
        structdata[thread_num].need_join = 1;
}

static int recv_eventloop(int evlen, char* rbuf, int *rbuf_len, struct epoll_event *child_events, int thnum)
{
        int ret = 0;
        char *buf;

        buf = &rbuf[*rbuf_len];

        for(int i = 0; i < evlen; i++) {
                ret = recv(child_events[i].data.fd, buf, length, 0);
                log_debug("ret %d", ret);
                if (ret == 0 || ret == -1) {
                        return -2;
                } else {
                        
                        *rbuf_len += ret;
                        
                        if (strcmp(rbuf, "exit\n") == 0) {
                                return -2;
                        }
                }
        }
        return 0;
}

static void do_parse(char *rawstr, int cur_len)
{
        int ret = 0;
        ret = find_first_text_off(rawstr, cur_len);
        idd(ret);

}

void handle_conn(int clientfd, server_state_t *server_state, int thread_num) {

        log_debug("thread %d", thread_num);
        
        int ret = 0;
        int child_epfd;
        int child_epfd_event_len;
        int buflen = 0;
        struct epoll_event child_event, child_events[CHILD_MAXEVENTS];

        epoll_init(&child_epfd);

        char *data = (char*)malloc(length);
        char *datatmp = (char*)malloc(length);

        if (data == NULL) {
                perror("malloc failed");
        } else {
                // memset(data, 0, length);
                // memset(datatmp, 0, length);
                
                log_info("handled fd %d", clientfd);

                epoll_watch(clientfd, child_epfd, &child_event);

                while (1) {
                        child_epfd_event_len = epoll_wait(child_epfd, child_events, 
                                                CHILD_MAXEVENTS, 1000);


                        log_debug("incmoming %d", child_epfd_event_len);
                        ret = recv_eventloop(child_epfd_event_len, data, &buflen, child_events,
                                                 thread_num);

                        
                        do_parse(data, buflen);
                        log_debug("thread %d says: %s", thread_num, 
                                                data);

                        if (*server_state->exit_now == 1 || ret == -2) {
                                thread_ask_to_exit:
                                log_info("thread exit");
                                break;
                        }
                }

        }
        
        setsignal_thread_free(server_state, thread_num);

        free(data);
        free(datatmp);
        close(child_epfd);
}