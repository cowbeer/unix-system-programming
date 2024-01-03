#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int i;
    for (i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return -1;
        }
        if (!pid) {
            printf("child process(%d)\n", getpid());
            return 0;
        }
    }

    for (;;) {
        printf("parent process is waiting for child process...\n");
        pid_t pid = wait(0);
        if (pid == -1) {
            if (errno != ECHILD) {
                perror("wait");
                return -1;
            }
            printf("no child process to wait\n");
            break;
        }
        else {
            printf("got child process(%d)\n", pid);
        }
    }

    return 0;
}

