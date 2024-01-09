#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sig(int signum) {
    printf("process(%d) received signal: %d\n", getpid(), signum);
}

void update_db(void) {
    for (int i = 0; i < 5; i++) {
        printf("process(%d) update %d record\n", getpid(), i+1);
        sleep(1);
    }
}

int main(void) {
    if (signal(SIGINT, handle_sig) == SIG_ERR) {
        perror("signal");
        return -1;
    }
    if (signal(50, handle_sig) == SIG_ERR) {
        perror("signal");
        return -1;
    }

    printf("process(%d) mask %d and %d\n", getpid(), SIGINT, 50);
    sigset_t set;
    if(sigemptyset(&set) == -1) {
        perror("sigemptyset");
        return -1;
    }
    if(sigaddset(&set, SIGINT) == -1) {
        perror("sigaddset");
        return -1;
    }
    if (sigaddset(&set, 50) == -1) {
        perror("sigaddset");
        return -1;
    }
    sigset_t old;
    if (sigprocmask(SIG_SETMASK, &set, &old) == -1) {
        perror("sigprocmask");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) {
        pid_t ppid = getppid();
        for (int i = 0; i < 5; i++) {
            printf("process(%d) send process(%d) %d\n", getpid(), ppid, 50);
            kill(ppid, 50);
        }
        return 0;
    }

    update_db();
    sigset_t pending;
    if (sigpending(&pending) == -1) {
        perror("sigpending");
        return -1;
    }
    if (sigismember(&pending, SIGINT)) {
        printf("process(%d) %d is  pending\n", getpid(), SIGINT);
    }
    if (sigismember(&pending, 50)) {
        printf("process(%d) %d is  pending\n", getpid(), 50);
    }
    if (sigprocmask(SIG_SETMASK, &old, NULL) == -1) {
        perror("sigprocmask");
        return -1;
    }

    return 0;
}


