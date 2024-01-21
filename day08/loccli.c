#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_FILE "/tmp/sock"

int main(void) {
    printf("client: create socket...\n");
    int sockfd = socket(AF_LOCAL,SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    printf("client: sockfd=%d\n", sockfd);
    printf("client: prepare address and bind...\n");
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, SOCK_FILE);
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        return -1;
    }
    printf("client: send data...\n");
    while(1) {
        printf("> ");
        char buf[1024] = {};
        gets(buf);
        if (!strcmp(buf, "!")) {
            break;
        }
        if (write(sockfd, buf, (strlen(buf)+1) * sizeof(buf[0])) == -1) {
            perror("write");
            return -1;
        }
        if (!strcmp(buf, "!!")) {
            break;
        }
    }
    printf("client: close...\n");
    if (close(sockfd) == -1) {
        perror("close");
        return -1;
    }
    printf("client: exit...\n");

    return 0;
}

