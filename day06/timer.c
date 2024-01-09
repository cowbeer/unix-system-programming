#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

int g_hour = 0, g_min = 0, g_sec = 0, g_msec = 0;

void handle_sigalrm(int signum) {
    printf("\r%02d:%02d:%02d.%03d", g_hour, g_min, g_sec, g_msec);
    if (++g_msec == 1000) {
        g_msec = 0;
        if (++g_sec == 60) {
            g_sec = 0;
            if (++g_min == 60) {
                g_min = 0;
                if (++g_hour == 24) {
                    g_hour = 0;
                }
            }
        }
    }
}

void handle_sigint(int signum) {
    static int run = 1;
    struct itimerval it = {{0,1000}, {0, 0}};
    if (run) {
        setitimer(ITIMER_REAL, &it, NULL);
    }
    else {
        printf("\n");
        g_hour = g_min = g_sec = g_msec = 0;
        it.it_value.tv_usec = 1;
        setitimer(ITIMER_REAL, &it, NULL);
    }
    run ^= 1;
}

int main(void) {
    setbuf(stdout, NULL);
    if (signal(SIGALRM, handle_sigalrm) == SIG_ERR) {
        perror("signal");
        return -1;
    }
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        return -1;
    }
    struct itimerval it = {{0, 1000}, {5, 0}};
    if (setitimer(ITIMER_REAL, &it, NULL) == -1) {
        perror("setitimer");
        return -1;
    }

    for(;;)
        pause();

    return 0;
}

