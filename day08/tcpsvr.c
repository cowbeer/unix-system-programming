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

void sigchild(int signum) {
    while(1) {
        pid_t pid = waitpid(-1, 0, WNOHANG);
        if (pid == -1) {
            if (errno != ECHILD) {
                perror("wait");
                exit(-1);
            }
            printf("server: all child processes exit.\n");
            break;
        }
        if (pid) {
            printf("server: process(%d) exit.\n", pid);
        }
        else {
            printf("server: no processes exit.\n");
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <Port>\n", argv[0]);
        return -1;
    }
    if (signal(SIGCHLD, sigchild) == SIG_ERR) {
        perror("signal");
        return -1;
    }
    printf("server: create socket...\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    printf("server: prepare address and bind...\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(atoi(argv[1]));
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("server: bind");
        return -1;
    }

    printf("server: listen...\n");
    if (listen(sockfd, 1024) == -1) {
        perror("listen");
        return -1;
    }

    while(1) {
        printf("server: accept connection...\n");
        struct sockaddr_in addrcli = {};
        socklen_t addrlen = sizeof(addrcli);
        int connfd = accept(sockfd, (struct sockaddr*)&addrcli, &addrlen);
        if (connfd == -1) {
            perror("accept");
            return -1;
        }

        printf("server: accepted %s:%d request...\n", inet_ntoa(addrcli.sin_addr), ntohs(addrcli.sin_port));
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return -1;
        }
        if (!pid) { // child process
            printf("process(%d) serve for: %s:%d...\n", getpid(), inet_ntoa(addrcli.sin_addr), ntohs(addrcli.sin_port));
            if (close(sockfd) == -1) {
                perror("sockfd");
                return -1;
            }

            while(1) {
                char buf[1024] = {};
                printf("process(%d) recv data ...\n", getpid());
                ssize_t rb = recv(connfd, buf, sizeof(buf), 0);
                if (rb == -1) {
                    perror("recv");
                    return -1;
                }
                if (rb == 0) { // client close
                    printf("process(%d): client closed.\n", getpid());
                    break;
                }
                printf("process(%d): send response...\n", getpid());
                if (send(connfd, buf, rb, 0) == -1) {
                    perror("send");
                    return -1;
                }
            }

            printf("process(%d): close connection...\n", getpid());
            if (close(connfd) == -1) {
                perror("close");
                return -1;
            }

            printf("process(%d) exit.\n", getpid());
            return 0;
        }

        if (close(connfd) == -1) {
            perror("close");
            return -1;
        }

    }

    printf("server: close...\n");
    if (close(sockfd) == -1) {
        perror("close");
        return -1;
    }
    printf("exit.\n");

    return 0;
}
