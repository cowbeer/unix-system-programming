#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    printf("PID: %d\n", getpid());
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) { // child process
        printf("In child process, PID: %d, PPID: %d\n", getpid(), getppid());
        return 0;
    }

    sleep(1);
    printf("PID: %d\n", getpid());
    printf("ps -efl | grep %d", pid);
    getchar();

    return 0;
}
