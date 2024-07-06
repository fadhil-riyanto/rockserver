#ifndef SERVER_H
#define SERVER_H

struct server_ctx {
    int *sockfd;
    int *epfd;
};

int sock_init(int port);
void sock_cleanup(int *sockfd);

#endif

