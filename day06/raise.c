#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int signum) {
    printf("process(%d) received SIGINT, signum: %d\n", getpid(), signum);
    exit(0);
}

int main(void) {
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        return -1;
    }
    printf("process(%d) will be killed\n", getpid());
    if (raise(SIGINT) == -1) {
        perror("raise");
        return -1;
    }

    for(;;);

    return 0;
}
