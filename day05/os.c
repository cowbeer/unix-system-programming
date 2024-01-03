#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    printf("ABC");

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (!pid) { // child process
        printf("XYZ\n");
        return 0;
    }

    sleep(1);
    printf("\n");

    return 0;
}

