#include "../header/threading.h"
#include "../header/server.h"

#include "../header/connection_handler.h"
#include <cstddef>
#include <sys/epoll.h>
#include "../header/epoll_watcher.h"
#include <cstring>
#include <string>
#include <stdio.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <netinet/in.h>
#include <unistd.h>
#include "config.c"
#include "utils/utils.h"
#include "../submodule/log.c-patched/src/log.h"
#include "../submodule/inih/ini.h"
#include "../header/inih_reader.h"


volatile sig_atomic_t exit_now = 0;
struct threading_ctx th[MAX_CONN];
struct server_ctx server_ctx;
int efd;
server_state_t server_state;


void exit_gracefully(struct server_ctx *server_ctx) {
    clean_thread(th);
    epoll_cleanup(server_ctx->epfd);
    sock_cleanup(server_ctx->sockfd);
    
}

void serverstate_install_req_data(volatile sig_atomic_t *exit_now, 
                                struct threading_ctx *threading_ctx) 
{
    server_state.exit_now = exit_now;
    server_state.threading_ch_ctx = threading_ctx;
}

static void signal_cb(int signo) {
    exit_now = 1;
    eventfd_t data = (eventfd_t)"exit now";
    eventfd_write(efd, data);
}

void _main() {
    struct epoll_event event, events[MAX_EVENTS];
    int epfd, event_counter;
    int freethread = 0;
    int acceptfd = 0;
    short ret;
    memset(&ret, 0, sizeof(ret));
    memset(&server_state, 0, sizeof(server_state_t));

    serverstate_install_req_data(&exit_now, th);

    ret = epoll_init(&epfd);
    if (ret == -1) {
        perror("epoll_init()");
    }

    efd = eventfd(0, 0);
    ret = epoll_watch(efd, epfd, &event);
    if (ret == -1) {
        perror("epoll watch");
    }
    
    int sockfd = sock_init(CONF_PORT);
    if (sockfd == -1) {
        fprintf(stderr, "socket error\n");
        sock_cleanup(&sockfd);
    }

    // all done
    server_ctx.sockfd = &sockfd;
    server_ctx.epfd = &epfd;

    ret = epoll_watch(sockfd, epfd, &event);
    if (ret == -1) {
        perror("epoll watch");
    }

    while(1) {
        
        clean_thread_queue(th);
        event_counter = epoll_wait(epfd, events, MAX_EVENTS, 1000);
        for(int i = 0; i < event_counter; i++) {
            if (events[i].data.fd == sockfd) {
                freethread = get_free_thread(th);
                if (freethread == -1) {
                    log_fatal("no space available, try increase config.c max connection");
                    // break;
                    std::string res = "SERVER_FULL";
                    dump_req(events[i].data.fd, res.c_str(), res.length());
                } else {
                    socklen_t socketsize = sizeof(struct sockaddr_in);
                    acceptfd = accept(sockfd, (struct sockaddr *)&th[freethread].clientaddr, 
                        &socketsize);
                    if (acceptfd == -1) {
                        perror("accept");
                    } else {
                        log_info("accepting ...");
                        fill_thread(th, freethread, handle_conn, acceptfd, &server_state, freethread);
                    }
                }
            } else if (events[i].data.fd == efd) {
                log_info("eventloop going to exit");
                exit_gracefully(&server_ctx);
                // break;
                goto _main_ret;
            }
            
        }
        // log_debug("exit_now == %d\n", exit_now);
        // if (exit_now == 1) {
        //     fprintf(stdout, "exitting");
        //     break;
        // }
    }
_main_ret:
    return;
}

int main() {
    if (signal(SIGINT, signal_cb) == SIG_ERR) {
        perror("signal err on set");
        return -1;
    }

    struct config pconfig;

    if (ini_parse("etc/rockserver/config.ini", inih_cb, &pconfig) < 0) {
        printf("Can't load 'test.ini'\n");
        return 1;
    }

    printf("Config loaded from '/etc/rockserver/config.ini': port=%d, max_conn=%d\n",
        pconfig.conf_port, pconfig.max_conn);

    
    // initialization
    init_thread(th);
    _main();
    return -1;
}