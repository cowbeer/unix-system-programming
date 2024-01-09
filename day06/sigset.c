#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
    sigset_t set;
    printf("sigset_t: %lu bytes\n", sizeof(set));
    sigfillset(&set);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    sigdelset(&set, SIGQUIT);

    if (sigismember(&set, SIGINT)) {
        printf("has SIGINT\n");
    }
    else {
        printf("no SIGINT\n");
    }

    if (sigismember(&set, SIGQUIT)) {
        printf("has SIGQUIT\n");
    }
    else {
        printf("no SIGQUIT\n");
    }

    return 0;
}
