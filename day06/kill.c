#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handle_sigint(int signum) {
    printf("process(%d) exit, signum: %d\n", getpid(), signum);
    exit(0);
}

int main(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) {
        printf("child(%d) is running...\n", getpid());
        if (signal(SIGINT, handle_sigint) == SIG_ERR) {
            perror("signal");
            return -1;
        }
        for(;;);
        return 0;
    }

    sleep(1);
    printf("parent(%d) will kill child(%d)\n", getpid(), pid);
    if (kill(pid, SIGINT) == -1){
        perror("kill");
        return -1;
    }

    if ((pid = wait(0)) == -1) {
        perror("wait");
        return -1;
    }
    printf("parent(%d): child(%d) was killed\n", getpid(), pid);

    return 0;
}
