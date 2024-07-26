#include <rocksdb/db.h>
#define USE_DEBUG_FN
#define USE_DEBUG_PRINT_LN

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
#include "../header/server.h"
#include "../header/rocksdb_ctx.h"
#include "rocksdb/options.h"
#include "utils/rocksdb_tcp_parser.h"
#include "../header/inih_reader.h"

#define char_length 65535
#define length (sizeof(char) * char_length)
#define real_char_length 
#define char_maxint_safety 4096  /* possible deprecated */
int pconfig_total_length = 0;

static void setsignal_thread_free(server_state_t *server_state, 
                                            int thread_num)
{
        log_debug("thread ended %d", thread_num);
        struct threading_ctx* structdata = (struct threading_ctx*)server_state->threading_ch_ctx;
        structdata[thread_num].state = 0;
        structdata[thread_num].need_join = 1;
}

static void flush_buffer(char *buf, int *len)
{
        memset(buf, 0, *len);
        *len = 0;
}

static int recv_eventloop(int evlen, char* rbuf, int *rbuf_len, struct epoll_event *child_events, int thnum,
        int need_read)
{
        int ret = 0;
        char *buf;

        buf = &rbuf[*rbuf_len];

        for(int i = 0; i < evlen; i++) {
                
                if (need_read == 0) {
                        flush_buffer(rbuf, rbuf_len);
                        send(child_events[i].data.fd, "overflow", strlen("overflow"), MSG_DONTWAIT);
                        return -3;
                }
                ret = recv(child_events[i].data.fd, buf, need_read, 0);
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

static void handleBufInput(char *src, int len, server_state_t *server_state, int clientfd)
{
        char *opcode = (char*)malloc(sizeof(char) * 4);
        char *value = (char*)malloc(sizeof(char) * server_state->pconfig->buffer_max_length);

        get_opcode(opcode, src);
        get_value(value, src, len, 3);

        printf("%s\n", opcode);
        printf("%s\n", value);

        free(opcode);
        free(value);
        
}

/* return 
  -1 : token not found, next read ... 
*/
static int do_parse(char *rawstr, int *cur_len, server_state_t *server_state, int clientfd)
{
        char *sanitized_buf = (char*)malloc(sizeof(char) * pconfig_total_length);

        int ret = 0;
        int orig_cur_len = 0;
        idd(pconfig_total_length);
        
        
do_extract:
        memset(sanitized_buf, 0, sizeof(char) * pconfig_total_length);
        orig_cur_len = *cur_len;
        ret = find_first_text_off(rawstr, *cur_len);
        // idd(ret);
        // idd(ret);
        if (ret == -1) {
                free(sanitized_buf);
                return -1;
        } else {
                /* note: first_strmv return allocated address based on ret + 1, any char array larger
                  than ret + 1 is unpredicted*/
                // idd(ret);


                first_strnmv(rawstr, sanitized_buf, ret);
                // __debug_str_ln(sanitized_buf, 30);

                /* zero str start from current offset, 
                  if array start from 0, so ret need to incremented again to 1 
                  int 4 from sizeof \r\n\r\n which 4 byte long */

                zerostr(rawstr, ret + 1, 4);

                /* move left the gap string 
                  ret + 1 because this is not array counting
                  4 from sizeof the zeroed \r\n\r\n, which also need to move left */
                left_string(rawstr, (ret + 1) + 4);

                /* set current off */
                
                *cur_len = orig_cur_len - (ret + 1 + 4);

                /* process our separated command */
                // __debug_str_ln(sanitized_buf, 30);
                handleBufInput(sanitized_buf, strlen(sanitized_buf), server_state, clientfd);

                if (ret != -1) 
                        goto do_extract;
        }
        free(sanitized_buf);
        
        return 0;
}




void handle_conn(int clientfd, server_state_t *server_state, int thread_num) {

        log_debug("thread %d", thread_num);
        
        int ret = 0;
        int child_epfd;
        int child_epfd_event_len;
        int buflen = 0;
        
        /* opcode space "key" space "buf"\r\n\r\n\0 */
        pconfig_total_length = 3 + 2 + server_state->pconfig->buffer_max_length + 4;

        struct epoll_event child_event, child_events[CHILD_MAXEVENTS];
        

        epoll_init(&child_epfd);

        /* alloc full length including termination chars */
        char *data = (char*)malloc(pconfig_total_length);

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
                                                 thread_num, (pconfig_total_length - buflen));

                        if (*server_state->exit_now == 1 || ret == -2) {
                                thread_ask_to_exit:
                                log_info("thread exit");
                                break;
                        }

                        /* safe area */
                        if (ret != -3) {
                                do_parse(data, &buflen, server_state, clientfd);
                        } else {
                                log_debug("overflow detected");
                                log_debug("%d", buflen);
                                
                        }
                        // idd(buflen);
                }

        }
        
        setsignal_thread_free(server_state, thread_num);

        free(data);
        // free(datatmp);
        close(child_epfd);
}