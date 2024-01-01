#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
// 1: can add write lock
// 0: can't add write lock
// -1: error
int test_write_lock(int fd, off_t start, off_t len) {
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    if (fcntl(fd, F_GETLK, &lock) == -1) {
        return -1;
    }
    if (lock.l_type == F_UNLCK) {
        return 1;
    }
    return 0;
}

int add_write_lock(int fd, off_t start, off_t len, int wait) {
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    return fcntl(fd, wait ? F_SETLKW : F_SETLK, &lock);
}

int ulock(int fd, off_t start, off_t len) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    return fcntl(fd, F_SETLK, &lock);
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        goto usage;

    int fd = open("nolock.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }

//    int unlock = 0;
//    do {
//        if ((unlock = test_write_lock(fd, 0, 0)) == -1) {
//            perror("test write");
//            return -1;
//        }
//        if (!unlock) {
//            printf("the file has been locked...\n");
//        }
//    } while (!unlock);
//
//    if (add_write_lock(fd, 0, 0, 0) == -1){
//        perror("add write lock");
//        return -1;
//    }
    while(add_write_lock(fd, 0, 0, 0) == -1) {
        if (errno != EACCES && errno != EAGAIN) {
            perror("add write lock");
            return -1;
        }
        printf("the file has been locked...\n");
    }

    size_t i, len = strlen(argv[1]);
    for (i = 0; i < len; ++i) {
        if (write(fd, &argv[1][i], sizeof(char)) == -1) {
            perror("write");
            return -1;
        }
        sleep(1);
    }

    if (ulock(fd, 0, 0) == -1) {
        perror("unlock");
        return -1;
    }

    close(fd);

    return 0;
usage:
    fprintf(stderr, "%s <string>\n", argv[0]);
    return -1;
}