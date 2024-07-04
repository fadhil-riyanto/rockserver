#include <stdio.h>
#include "header/connection_handler.h"

void handle_conn(int clientfd) {
    printf("handled fd %d\n", clientfd);
}