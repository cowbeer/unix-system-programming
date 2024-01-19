#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_FILE "fifo"

int main(void) {
    printf("create fifo...\n");
    if (mkfifo(FIFO_FILE, 0666) == -1) {
        perror("mkfifo");
        return -1;
    }

    printf("open fifo...\n");
    int fd = open(FIFO_FILE, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    printf("send msg...\n");
    for (;;) {
        printf("> ");
        char buf[1024] = {};
        gets(buf);
        if (!strcmp(buf, "!")) {
            break;
        }
        if (write(fd, buf, strlen(buf)+1) == -1) {
            perror("write");
            return -1;
        }
    }

    printf("close fifo...\n");
    close(fd);
    printf("delete fifo...\n");
    if (unlink(FIFO_FILE) == -1) {
        perror("unlink");
        return -1;
    }
    printf("ok...\n");
    return 0;
}

