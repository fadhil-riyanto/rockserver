#include <rocksdb/db.h>
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
#include "../header/server.h"
#include "../header/rocksdb_ctx.h"
#include "rocksdb/options.h"
#include "utils/rocksdb_tcp_parser.h"

#define char_length 65535
#define length (sizeof(char) * char_length)
#define char_maxint_safety (char_length - 4096)

static void setsignal_thread_free(server_state_t *server_state, 
                                            int thread_num)
{
        log_debug("thread ended %d", thread_num);
        struct threading_ctx* structdata = (struct threading_ctx*)server_state->threading_ch_ctx;
        structdata[thread_num].state = 0;
        structdata[thread_num].need_join = 1;
}

/* return -3 on pass, -2 on exit */

static int recv_eventloop(int evlen, char* rbuf, int *rbuf_len, struct epoll_event *child_events, int thnum,
                                int avlen)
{
        int ret = 0;
        char *buf;

        buf = &rbuf[*rbuf_len];

        for(int i = 0; i < evlen; i++) {

                /* buffer will overflow here because the subtraction is less than zero  */
                if (avlen < 0) {
                        sdd("ev les than 0 caught")
                        return -3;
                }
                        
                ret = recv(child_events[i].data.fd, buf, avlen, 0);
                // idd(avlen);
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
        
        

        // log_debug("req %s\n", src);

        // char *err = NULL;
        // rocksdb_writeoptions_t *writeoptions = rocksdb_writeoptions_create();
        // const char key[] = "key";
        // const char *value = "value";
        // rocksdb_put(server_state->db, writeoptions, key, strlen(key), value, strlen(value) + 1,
        //         &err);

        // return;
        struct parse_res res;

        alloc_parse(&res);
        parse(src, len, &res);

        // idd();
        // __debug_parser(res.op_code, res.op1, res.op2);
        if (res.op_code == RCK_COMMAND_SET) {
                
                server_state->db->Put(rocksdb::WriteOptions(), res.op1, res.op2);
                // send(clientfd, "ok\r\n\r\n", strlen("ok\r\n\r\n"), MSG_DONTWAIT);
        }
                

        if (res.op_code == RCK_COMMAND_GET) {
                std::string value;
                rocksdb::Status s = server_state->db->Get(rocksdb::ReadOptions(), res.op1, &value);
                
                send(clientfd, value.c_str(), strlen(value.c_str()), MSG_DONTWAIT);
        }
        
        free_parse(&res);
}

static void do_parse(char *rawstr, int *cur_len, server_state_t *server_state, int clientfd)
{
        char *sanitized_buf = (char*)malloc(sizeof(char) * char_length);
        int ret = 0;
        int orig_cur_len = 0;
        
        
do_extract:
        orig_cur_len = *cur_len;
        ret = find_first_text_off(rawstr, *cur_len);
        // idd(ret);
        // idd(ret);
        if (ret == -1) {
                free(sanitized_buf);
                return;
        } else {
                /* note: first_strmv return allocated address based on ret + 1, any char array larger
                  than ret + 1 is unpredicted*/
                // idd(ret);

                first_strnmv(rawstr, sanitized_buf, ret);
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
                // __debug_str(rawstr, 30);
                *cur_len = orig_cur_len - (ret + 1 + 4);

                /* process our separated command */
                handleBufInput(sanitized_buf, strlen(sanitized_buf), server_state, clientfd);

                if (ret != -1) 
                        goto do_extract;
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
                                                 thread_num, (char_maxint_safety - buflen));
                        // idd(ret);
                        // if (ret == -3) {
                        //         log_debug("danger batch send ENOMEM, start parsing..., av: %d ret %d", buflen, ret);
                        //         do_parse(data, &buflen, server_state, clientfd);
                        //         break;
                        // }
                                

                        // __debug_str(data, 100);

                        if (*server_state->exit_now == 1 || ret == -2) {
                                thread_ask_to_exit:
                                log_info("thread exit");
                                break;
                        }

                        /* safe area */
                        do_parse(data, &buflen, server_state, clientfd);
                        // idd(buflen);
                }

        }
        
        setsignal_thread_free(server_state, thread_num);

        free(data);
        // free(datatmp);
        close(child_epfd);
}