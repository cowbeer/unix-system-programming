#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void siginfo(int signum, siginfo_t* si, void* pv) {
    printf("%d: received signal(%d) from %d with data: %d\n", getpid(), signum, si->si_pid, si->si_value.sival_int);
    usleep(100);
}

int main(void) {
    //int signum = 50;
    int signum = SIGINT;
    struct sigaction act = {};
    act.sa_sigaction = siginfo;
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    if (sigaction(signum, &act, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) {
        pid_t ppid = getppid();
        for (int i = 0; i< 10; ++i) {
            sigval_t sv;
            sv.sival_int = i;
            printf("%d: send %d signal(%d) with data:%d\n", getpid(), ppid, signum, sv.sival_int);
            if (sigqueue(ppid, signum, sv) == -1) {
                perror("sigqueue");
                return -1;
            }
        }
        printf("%d exit\n", getpid());
        return 0;
    }

    if ((pid = wait(0)) == -1) {
        perror("wait");
        return -1;
    }
    printf("%d exit\n", getpid());


    return 0;
}
