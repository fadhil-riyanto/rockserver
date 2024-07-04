#include "header/server.h"
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int sock_init(int port) {
    int sockfd, connfd;
    struct sockaddr_in sa_in;
    struct sockaddr_in sa_client_in;
    int sin_size;
    int sockoptval;

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
    sa_in.sin_port = htons(9880);
    bzero(&sa_in.sin_zero, 8);

    if (bind(sockfd, (struct sockaddr*)&sa_in, sizeof(sa_in)) == -1) {
        perror("bind");
        goto err;
    }

    if (listen(sockfd, 100) == -1) {
        perror("listen");
        goto err;
    }

    return sockfd;

err:
    close(sockfd);
    return -1;


}