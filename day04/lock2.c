#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void print_lock(struct flock lock) {
    if (lock.l_type == F_UNLCK) {
        printf("no lock\n");
    } else {
        printf("process: %u, ", lock.l_pid);
        switch (lock.l_whence) {
            case SEEK_SET:
                printf("to head ");
                break;
            case SEEK_CUR:
                printf("to current ");
                break;
            case SEEK_END:
                printf("to end ");
                break;
        }

        printf("pos: %ld, len: %ld", lock.l_start, lock.l_len);
        switch (lock.l_type) {
            case F_RDLCK:
                printf(", read lock\n");
                break;
            case F_WRLCK:
                printf(", write lock\n");
                break;
        }
    }
}

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

int read_lock_test(int fd, off_t start, off_t len) {
    struct flock lock;
    lock.l_type= F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    if (fcntl(fd, F_GETLK, &lock)==-1) {
        return -1;
    }
    print_lock(lock);

    return 0;
}

int write_lock_test(int fd, off_t start, off_t len) {
    struct flock lock;
    lock.l_type= F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    if (fcntl(fd, F_GETLK, &lock) == -1) {
        return -1;
    }
    print_lock(lock);

    return 0;
}

int main() {
    int fd = open("lock.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    printf("read lock test CDEF: ");
    if (read_lock_test(fd, 2, 4) == -1) {
        printf("%m\n");
    }
    printf("add read lock for CDEF\n");
    if (read_lock(fd, 2, 4, 0) == -1) {
        printf("%m\n");
    } else {
        printf("add read lock for CDEF, ok\n");
        unlock(fd, 2, 4);
    }

    printf("write lock test CDEF: ");
    if (write_lock_test(fd, 2, 4) == -1) {
        printf("%m\n");
    }
    printf("add write lock for CDEF\n");
    if (write_lock(fd, 2, 4, 0) == -1) {
        printf("%m\n");
    } else {
        printf("add write lock for CDEF, ok\n");
        unlock(fd, 2, 4);
    }

    printf("read lock test KLMN: ");
    if (read_lock_test(fd, 10, 4) == -1) {
        printf("%m\n");
    }
    printf("add read lock for KLMN\n");
    if (read_lock(fd, 10, 4, 0) == -1) {
        printf("%m\n");
    } else {
        printf("add read lock for KLMN, ok\n");
        unlock(fd, 10, 4);
    }

    printf("write lock test KLMN: ");
    if (write_lock_test(fd, 10, 4) == -1) {
        printf("%m\n");
    }
    printf("add write lock for KLMN\n");
    if (write_lock(fd, 10, 4, 0) == -1) {
        printf("%m\n");
    } else {
        printf("add write lock for KLMN, ok\n");
        unlock(fd, 10, 4);
    }

    printf("waiting for KLMN write lock...\n");
    if (write_lock(fd, 10, 4, 1) == -1) {
        printf("%m\n");
    } else {
        printf("add write lock for KLMN, ok\n");
        unlock(fd, 10, 4);
    }

    close(fd);
    return 0;
}
