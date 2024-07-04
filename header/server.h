#ifndef SERVER_H
#define SERVER_H

struct server_status {
    short *exit_now;

}

int sock_init(int port);
void sock_cleanup(int *sockfd);

#endif

