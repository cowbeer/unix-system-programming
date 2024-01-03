#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(void){
    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return -1;
        }
        if (!pid) {
            printf("child: %d\n", getpid());
            return 0;
        }
    }

    printf("parent: %d is waiting for child...\n", getpid());
    for(;;) {
        pid_t pid = waitpid(-1, 0, WNOHANG); // non-blocking
        if (pid == -1) {
            if (errno != ECHILD) {
                perror("wait");
                return -1;
            }
            printf("no child to wait\n");
            break;
        }
        if (pid) {
            printf("got child: %d\n", pid);
        }
        else {
            printf("do other things...\n");
        }
    }
    return 0;
}
