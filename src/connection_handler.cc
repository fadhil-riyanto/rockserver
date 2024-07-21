#define USE_DEBUG_FN

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <regex.h>
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
#define char_maxint_safety 2000

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

static void do_parse(char *rawstr, int *cur_len)
{
        char *sanitized_buf;
        int ret = 0;
        int orig_cur_len = *cur_len;
        __debug_str(rawstr, 30);
        // idd(*cur_len)
        
        ret = find_first_text_off(rawstr, *cur_len);
        // idd(ret);
        if (ret == -1) 
                return;
        else {
                /* note: first_strmv return allocated address based on ret + 1, any char array larger
                  than ret + 1 is unpredicted*/
                // idd(ret);
                sanitized_buf = first_strnmv(rawstr, ret);
                // __debug_str(sanitized_buf, 30);

                /* zero str start from current offset, 
                  if array start from 0, so ret need to incremented again to 1 
                  int 4 from sizeof \r\n\r\n which 4 byte long */
                zerostr(rawstr, ret + 1, 4);

                /* move left the gap string 
                  ret + 1 because this is not array counting
                  4 from sizeof the zeroed \r\n\r\n, which also need to move left */
                left_string(rawstr, (ret + 1) + 4);

                /* set current off */
                __debug_str(rawstr, 30);
                *cur_len = orig_cur_len - (ret + 1 + 4);
               

                
                
                
        }
        free(sanitized_buf);



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

        if (data == NULL) {
                perror("malloc failed");
        } else {
                memset(data, 0, length);
                
                log_info("handled fd %d", clientfd);

                epoll_watch(clientfd, child_epfd, &child_event);

                while (1) {
                        child_epfd_event_len = epoll_wait(child_epfd, child_events, 
                                                CHILD_MAXEVENTS, 1000);
                        ret = recv_eventloop(child_epfd_event_len, data, &buflen, child_events,
                                                 thread_num);

                        if (*server_state->exit_now == 1 || ret == -2) {
                                thread_ask_to_exit:
                                log_info("thread exit");
                                break;
                        }

                        /* safe area */
                        do_parse(data, &buflen);
                        // idd(buflen);
                }

        }
        
        setsignal_thread_free(server_state, thread_num);

        free(data);
        // free(datatmp);
        close(child_epfd);
}