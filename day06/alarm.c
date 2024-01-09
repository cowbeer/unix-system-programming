#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigalrm(int signum) {
    printf("received SIGALRM, signum: %d\n", signum);
    exit(0);
}

int main(void) {
    if (signal(SIGALRM, handle_sigalrm) == SIG_ERR) {
        perror("signal");
        return -1;
    }

    unsigned int remain = alarm(10);
    printf("alarm 10s...\n");
    printf("sleep 2s...\n");
    sleep(2);
//    remain = alarm(5);
//    printf("alarm 5s, remain %ds\n", remain);

    remain = alarm(0);
    printf("cancel alarm, remain: %ds\n", remain);

    for(;;);

    return 0;
}


