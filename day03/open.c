#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void) {
    int fd1 = open("open.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd1 == -1) {
        perror("open");
        return -1;
    }

    printf("fd1 = %d\n", fd1);

    close(fd1);
    return 0;
}
