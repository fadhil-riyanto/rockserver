#include "header/server.h"
#include "header/threading.h"
#include "header/connection_handler.h"
#include "header/epoll_watcher.h"
#include <cstring>
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "config.c"

volatile sig_atomic_t exit_now = 0;
struct threading_ctx th[MAX_CONN];
struct server_status server_status;

static void signal_cb(int signo) {
    exit_now = 1;
    server_status.exit_now = (short *)&exit_now;
}

void exit_gracefully(int *sockfd) {
    sock_cleanup(sockfd);
    clean_thread(th);
}

void _main() {
    int epfd;
    int freethread = 0;
    int acceptfd = 0;
    short ret;
    memset(&ret, 0, sizeof(ret));

    ret = epoll_init(&epfd);

    int sockfd = sock_init(CONF_PORT);
    if (sockfd == -1) {
        fprintf(stderr, "socket error\n");
        sock_cleanup(&sockfd);
    }

    while(1) {
        if (exit_now) {
            fprintf(stdout, "exitting");
            exit_gracefully(&sockfd);
            break;
        }

        freethread = get_free_thread(th);
        if (freethread == -1) {
            fprintf(stderr, "no space available, try increase config.c max connection");
            exit_gracefully(&sockfd);
            break;
        } else {
            socklen_t socketsize = sizeof(struct sockaddr_in);
            acceptfd = accept(sockfd, (struct sockaddr *)&th[freethread].clientaddr, &socketsize);
            if (acceptfd == -1) {
                perror("accept");
            } else {
                fill_thread(th, freethread, handle_conn, acceptfd);
            }
            
        }

        printf("%d\n", freethread);

        
        
    }
}

int main() {
    if (signal(SIGINT, signal_cb) == SIG_ERR) {
        perror("signal err on set");
        return -1;
    }
    
    // initialization
    init_thread(th);
    _main();
    return -1;
}