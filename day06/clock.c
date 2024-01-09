#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void handle_sigalrm(int signum) {
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    printf("\r%02d:%02d:%02d\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
    alarm(1);
}

int main(void) {
    setbuf(stdout, NULL);
    if (signal(SIGALRM, handle_sigalrm) == SIG_ERR) {
        perror("signal");
        return -1;
    }

    handle_sigalrm(SIGALRM);
    for (;;) {
        // do other things
    }

    return 0;
}

