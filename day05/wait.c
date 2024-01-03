#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) { // child process
        int status = 0x12345678;
        printf("child process(%d): status(%#x)\n", getpid(), status);
        return status;
    }

    printf("parent process is waiting for child process...\n");
    int status;
    printf("%#x\n",status);
    printf("parent process got child process(%d) status: %#x\n", pid, WEXITSTATUS(status));

    return 0;
}