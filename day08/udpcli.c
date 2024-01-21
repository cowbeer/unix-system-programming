#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <IP> <Port>\n", argv[0]);
        return -1;
    }

    printf("client: create dgram socket...\n");
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    printf("client: prepare address and connect...\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    printf("client: send data and receive response...\n");
    while(1) {
        printf("> ");
        char buf[1024] = {};
        gets(buf);
        if (!strcmp(buf, "!")) {
            break;
        }
        if (sendto(sockfd, buf, (strlen(buf)+1)*sizeof(buf[0]),0, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("sendto");
            return -1;
        }
        struct sockaddr_in addrsvr = {};
        socklen_t addrlen = sizeof(addrsvr);
        ssize_t rb = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&addrsvr, &addrlen);
        if (rb == -1) {
            perror("recvfrom");
            return -1;
        }
        printf("< %s\n", buf);
    }

    printf("client: close.\n");
    if (close(sockfd) == -1) {
        perror("close");
        return -1;
    }
    printf("client exit.\n");

    return 0;
}
