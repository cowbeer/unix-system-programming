#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <IP> <Port>\n", argv[0]);
        return -1;
    }
    printf("client: create network socket...\n");
    int sockfd = socket(AF_INET,SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    printf("client: sockfd=%d\n", sockfd);
    printf("client: prepare address and bind...\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

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

