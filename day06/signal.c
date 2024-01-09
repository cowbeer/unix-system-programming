#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_sigkill(int signum) {
    printf("process(%d) received SIGKILL, signum: %d\n", getpid(), signum);
}

void handle_sigint(int signum) {
    signal(SIGINT, SIG_DFL);
    printf("process(%d) received SIGINT, signum: %d\n", getpid(), signum);
    signal(SIGINT, handle_sigint);
}

int main(void) {
    // register handle func in kernel
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        return -1;
    }
    // SIGKILL: can't handle and ignore
//    if (signal(SIGKILL, handle_sigkill) == SIG_ERR) {
//        perror("signal");
//        return -1;
//    }

    printf("Can't quit to enter Ctrl+C\n");

    for(;;);

    return 0;
}


