#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


#define FIFO_FILE "fifo"

int main(void) {
    printf("open fifo...\n");
    int fd = open(FIFO_FILE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    printf("receive msg...\n");
    for (;;) {
        char buf[1024] = {};
        size_t rb = read(fd, buf, sizeof(buf));
        if (rb == -1) {
            perror("read");
            return -1;
        }
        if (!rb) {
            break;
        }
        printf("< %s\n", buf);
    }
    printf("close fifo...\n");
    if (close(fd) == -1) {
        perror("close");
        return -1;
    }
    printf("ok...\n");
    return 0;
}
