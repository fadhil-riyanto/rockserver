#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "utils.h"
#include <stdio.h>

void dump_req(int sockfd, const char* reason, size_t len) 
{
        short ret;
        struct sockaddr_in clientaddr_dumped;

        socklen_t socketsize = sizeof(struct sockaddr_in);
        int acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr_dumped, 
            &socketsize);

        ret = send(acceptfd, (char*)reason, len, 0);

        if (ret == -1) {
                perror("send on dump_req()");
        }
        close(acceptfd);
}