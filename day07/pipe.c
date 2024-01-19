#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("parent process create pipe...\n");
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    printf("parent process create child process...\n");
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) {
        printf("child process close pipefd[1]...\n");
        close(pipefd[1]);
        printf("child process receive data...\n");
        for (;;) {
            char buf[1024];
            ssize_t rb = read(pipefd[0], buf, sizeof(buf));
            if (rb == -1) {
                perror("read");
                return -1;
            }
            if (!rb) {
                break;
            }
            puts(buf);
        }
        printf("child process close pipefd[0]...\n");
        close(pipefd[0]);
        printf("child process exit...\n");
        return 0;
    }
    printf("parent process close pipefd[0]...\n");
    close(pipefd[0]);
    printf("parent process send msg...\n");
    for (;;) {
        char buf[1024] = {};
        gets(buf);
        if (!strcmp(buf, "!")) {
            break;
        }
        if (write(pipefd[1], buf, strlen(buf)+1) == -1) {
            perror("write");
            return -1;
        }
    }
    printf("parent process close pipefd[1]...\n");
    close(pipefd[1]);
    if (wait(0) == -1) {
        perror("wait");
        return -1;
    }
    printf("parent process exit...\n");
    return 0;
}
