#include "header/server.h"
#include "header/threading.h"
#include <stdio.h>
#include <signal.h>
#include "config.c"

volatile sig_atomic_t exit_now = 0;
struct threading_ctx th[MAX_CONN];

static void signal_cb(int signo) {
    exit_now = 1;
}

void exit_gracefully(int *sockfd) {
    sock_cleanup(sockfd);
    clean_thread(th);
}

void handle_conn() {
    printf("handled\n");
}

void _main() {
    int freethread = 0;
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
            // if ((th[freethread].handler))
            fill_thread(th, freethread, handle_conn);
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