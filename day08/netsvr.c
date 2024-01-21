#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <Port>\n", argv[0]);
        return -1;
    }

    printf("server: create network socket...\n");
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    printf("server: sockfd=%d\n", sockfd);
    printf("server: prepare address and bind...\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return -1;
    }

    printf("server: receive data...\n");
    while(1) {
        char buf[1024] = {};
        ssize_t rb = read(sockfd, buf, sizeof(buf));
        if (rb == -1) {
            perror("read");
            return -1;
        }

        if (!strcmp(buf, "!!")) {
            break;
        }

        printf("< %s\n", buf);
    }

    printf("server: close...\n");
    if (close(sockfd) == -1) {
        perror("close");
        return -1;
    }

    printf("server: exit...\n");

    return 0;
}
