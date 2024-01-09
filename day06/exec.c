#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int signum) {
    printf("process(%d) received SIGINT, signum: %d\n", getpid(), signum);
}
int main(void) {
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        return -1;
    }
    if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) {
        perror("signal");
        return -1;
    }

    pid_t pid = vfork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) {
        printf("child process(%d) is running...\n", getpid());
        // loop: gcc loop.c -o loop
        if (execl("./loop", "./loop", NULL) == -1) {
            perror("execl");
            return -1;
        }
        return 0;
    }

    sleep(1);
    printf("parent process(%d) will return...\n", getpid());

    return 0;
}