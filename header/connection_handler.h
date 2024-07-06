#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <signal.h>

void handle_conn(int clientfd, volatile sig_atomic_t *signal_exit_now);

#endif