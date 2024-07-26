#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "server.h"

enum rockserver_opcode {
        OPCODE_SET,
        OPCODE_GET,
        OPCODE_DEL
        
};

struct state_data {
        rockserver_opcode opcode;
        char *op1;
        char *op2;
        int complete;
};

void state_data_init(struct state_data *state_data, server_state_t *server_state);
void state_data_destroy(struct state_data *state_data);

void handle_conn(int clientfd, server_state_t *server_state, int thread_num);

#endif