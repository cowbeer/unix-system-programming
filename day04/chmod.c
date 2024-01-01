#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
int main() {
    int fd = open("chmod.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    /*
     *   S       U        G        O
     *   7       6        5        4
     *  421     420      401      400
     *  ugt     rw-      r-x      r--  ----->  rwSr-sr-T
     *
     * */
//    if (fchmod(fd, 07654) == -1) {
    if (fchmod(fd, S_ISUID | S_ISGID | S_ISVTX |
        S_IRUSR | S_IWUSR |
        S_IRGRP | S_IXGRP |
        S_IROTH) == -1) {
        perror("fchmod");
        return -1;
    }

    close(fd);

    return 0;
}