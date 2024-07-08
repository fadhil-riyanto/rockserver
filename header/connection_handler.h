#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "server.h"

void handle_conn(int clientfd, server_state_t *server_state);

#endif