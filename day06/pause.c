#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handle_sigint(int signum) {
    printf("process(%d) received SIGINT, signum: %d\n", getpid(), signum);
}

int main(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) {
        if (signal(SIGINT, handle_sigint) == SIG_ERR) {
            perror("signal");
            return -1;
        }
        printf("child(%d) will sleep...\n", getpid());
        pause();
        printf("child(%d) waked up\n", getpid());
        return 0;
    }

    sleep(1);
    printf("process(%d) will send child(%d) SIGINT\n", getpid(), pid);
    if (kill(pid, SIGINT) == -1) {
        perror("kill");
        return -1;
    }
    if ((pid = wait(0)) == -1) {
        perror("wait");
        return -1;
    }
    printf("process(%d): child(%d) exited\n", getpid(), pid);

    return 0;
}
