#include "header/threading.h"
#include <stdio.h>
#include <signal.h>
#include "config.c"

volatile sig_atomic_t exit_now = 0;
struct threading_ctx th[MAX_CONN];

static void signal_cb(int signo) {
    exit_now = signo;
}


void _main() {
    printf("%d\n", get_free_thread(th));
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