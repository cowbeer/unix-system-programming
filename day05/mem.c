#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global = 100;

int main(void) {
    int local = 200;
    char* heap = (char*)malloc(256 * sizeof(char));
    sprintf(heap, "ABC");
    printf("parent: %d %d %s\n", global, local, heap);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (!pid) { // child process
        global++;
        local++;
        sprintf(heap, "XYZ");
        printf("child: %d %d %s\n", global, local, heap);
        free(heap);
        return 0;
    }

    sleep(1);
    printf("parent: %d %d %s\n", global, local, heap);
    free(heap);

    return 0;
}
