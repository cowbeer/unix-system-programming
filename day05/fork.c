#include <stdio.h>
#include <unistd.h>


int main(void) {
    printf("pid: %d\n", getpid());

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 0;
    }

    if (!pid) {
        // child process
        printf("%d is %d child\n", getpid(), getppid());
        return 0;
    }

    printf("%d is %d parent\n", getpid(), pid);
    sleep(1);

    return 0;
}
