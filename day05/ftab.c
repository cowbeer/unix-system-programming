#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("ftab.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    const char* text = "hello, world !";
    if (write(fd, text, strlen(text) * sizeof(text[0])) == -1) {
        perror("write");
        return -1;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (!pid) { // child process
        if (lseek(fd, -7, SEEK_CUR) == -1) {
            perror("lseek");
            return -1;
        }
        close(fd);
        return 0;
    }
    sleep(1);
    text = "Linux";
    if (write(fd, text, strlen(text)/sizeof(text[0])) == -1) {
        perror("write");
        return -1;
    }
    close(fd);
    return 0;
}
