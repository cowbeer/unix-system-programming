#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <Port>\n", argv[0]);
        return -1;
    }

    printf("server: create dgram socket...\n");
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("sockfd");
        return -1;
    }

    printf("server: prepare address and bind...\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return -1;
    }

    while(1) {
        printf("server: receive request...\n");
        char buf[1024] = {};
        struct sockaddr_in addrcli = {};
        socklen_t addrlen = sizeof(addrcli);
        ssize_t rb = recvfrom(sockfd, buf, sizeof(buf),0, (struct sockaddr*)&addrcli, &addrlen);
        if (rb == -1) {
            perror("recvfrom");
            return -1;
        }
        printf("server: reply to %s:%d\n", inet_ntoa(addrcli.sin_addr), ntohs(addrcli.sin_port));
        if (sendto(sockfd, buf, rb, 0, (struct sockaddr*)&addrcli, addrlen) == -1 ) {
            perror("sendto");
            return -1;
        }
    }
    printf("server: close.\n");
    if (close(sockfd) == -1) {
        perror("close");
        return -1;
    }
    printf("server: exit.\n");

    return 0;
}

