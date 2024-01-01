#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
//    mode_t odl = umask(
//            S_IWUSR | S_IXUSR |
//            S_IWGRP | S_IXGRP |
//            S_IWOTH | S_IXOTH);
    mode_t old = umask(0333);
    int fd = open("umask.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    printf("old mode: %d\n", old);
    close(fd);
    umask(old);

    return 0;
}
