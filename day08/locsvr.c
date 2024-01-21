#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_FILE "/tmp/sock"

int main(void) {
    printf("server: create local socket...\n");
    int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    printf("server: sockfd=%d\n", sockfd);
    printf("server: prepare address and bind...\n");
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, SOCK_FILE);
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

    printf("server: remove sock file...\n");
    if (unlink(SOCK_FILE) == -1) {
        perror("unlink");
        return -1;
    }

    printf("server: exit...\n");

    return 0;
}
