#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint1(int signum) {
    pid_t pid = getpid();
    printf("received SIGINT(%d)\n", signum);
    sleep(5);
    printf("wake up\n");
}

void handle_sigint2(int signum, siginfo_t* si, void* pv) {
    pid_t pid = getpid();
    printf("received SIGINT(%d) from %d\n", signum, si->si_pid);
}

int main(void) {
    printf("Ctrl+C and Ctrl+\\\n");
    struct sigaction act = {};

    //printf("mask SIGINT, unmask SIGQUIT\n");
    //act.sa_handler = handle_sigint1;

    //printf("mask SIGINT and SIGQUIT\n");
    //act.sa_handler = handle_sigint1;
    //sigemptyset(&act.sa_mask);
    //sigaddset(&act.sa_mask, SIGQUIT);

    //printf("unmask mask SIGINT and SIGQUIT\n");
    //act.sa_handler = handle_sigint1;
    //act.sa_flags = SA_NOMASK;

    //printf("unmask SIGINT, mask SIGQUIT\n");
    //act.sa_handler = handle_sigint1;
    //act.sa_flags = SA_NOMASK;
    //sigemptyset(&act.sa_mask);
    //sigaddset(&act.sa_mask, SIGQUIT);

    //printf("with data\n");
    //act.sa_sigaction = handle_sigint2;
    //act.sa_flags = SA_SIGINFO;

    printf("once");
    act.sa_handler = handle_sigint1;
    act.sa_flags = SA_RESETHAND;

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    for(;;) {
        pause();
    }

    return 0;
}

