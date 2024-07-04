#include "header/server.h"
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "config.c"

int sock_init(int port) {
    int sockfd;
    struct sockaddr_in sa_in;
    int sockoptval = 0;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return -1;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(sockoptval)) == -1) {
        perror("setsockopt");
        goto err;
    }

    sa_in.sin_family = AF_INET;
    if (inet_pton(AF_INET, "127.0.0.1", &sa_in.sin_addr) == -1) {
        perror("inet pton");
        goto err;
    }
    sa_in.sin_port = htons(port);
    bzero(&sa_in.sin_zero, 8);

    if (bind(sockfd, (struct sockaddr*)&sa_in, sizeof(sa_in)) == -1) {
        perror("bind");
        goto err;
    }

    if (listen(sockfd, MAX_CONN) == -1) {
        perror("listen");
        goto err;
    }

    return sockfd;

err:
    close(sockfd);
    return -1;


}

void sock_cleanup(int *sockfd) {
    close(*sockfd);
}

