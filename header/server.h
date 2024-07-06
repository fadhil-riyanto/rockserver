#ifndef SERVER_H
#define SERVER_H

int sock_init(int port);
void sock_cleanup(int *sockfd);

#endif

