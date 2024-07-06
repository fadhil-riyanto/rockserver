#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "header/connection_handler.h"

void handle_conn(int clientfd) {

    size_t length = sizeof(char) * 65535;
    ssize_t ret;

    char *data = (char*)malloc(length);
    if (data == NULL) {
        perror("malloc failed");
    } else {
        memset(data, 0, length);

        printf("handled fd %d\n", clientfd);
        while (1) {
            ret = recv(clientfd, data, length, 0);
            if (ret == 0) {
                break;
            } else {
                data[ret] = '\0';

                printf("client ret recv: %s", data);
                printf("%d\n", clientfd);
                if (strcmp(data, "exit\n") == 0) {
                    break;
                }
            }
            
        }

    }
    
    free(data);
    close(clientfd);
}