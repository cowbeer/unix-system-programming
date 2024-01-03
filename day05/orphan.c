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
        sleep(2);
        printf("child: %d, parent: %d\n", getpid(), getppid());
        return 0;
    }
    printf("PID: %d\n", getpid());

    return 0;
}
