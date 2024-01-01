#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// add read lock
int read_lock(int fd, off_t start, off_t len, int wait) {
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    return fcntl(fd, wait ? F_SETLKW : F_SETLK, &lock);
}
// add write lock
int write_lock(int fd, off_t start, off_t len, int wait) {
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    return fcntl(fd, wait ? F_SETLKW : F_SETLK, &lock);
}

int unlock(int fd, off_t start, off_t len) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    return fcntl(fd, F_SETLK, &lock);
}

int main() {
    printf("process(PID): %u\n", getpid());
    int fd = open("lock.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    const char* text = "ABCDEFGHIJKLMNOPQR";
    if (write(fd, text, strlen(text) * sizeof(char)) == -1) {
        perror("write");
        return -1;
    }

    // add read lock: EFGH
    printf("read lock: EFGH\n");
    if (read_lock(fd, 4, 4, 0) == -1) {
        printf("failed to add read lock: %m\n");
        return -1;
    }
    printf("add read lock, ok\n");

    printf("write lock: MNOP\n");
    if (write_lock(fd, 12, 4, 0)) {
        printf("failed to add write lock: %m\n");
        return -1;
    }
    printf("add write lock, ok\n");

    printf("enter to unlock: MN");
    getchar();
    if (unlock(fd, 12, 2) == -1) {
        printf("failed to unlock MN: %m\n");
        return -1;
    }

    printf("enter to unlock: EFGH");
    getchar();
    if (unlock(fd, 4, 4) == -1) {
        printf("failed to unlock EFGH: %m\n");
        return -1;
    }

    close(fd);

    return 0;
}