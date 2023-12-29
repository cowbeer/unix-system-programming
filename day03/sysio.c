#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("sysio.dat", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    unsigned int i;
    for (i = 0; i< 1000000; i++) {
         write(fd, &i, sizeof(i));
    }

    close(fd);
    return 0;
}
