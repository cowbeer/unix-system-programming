#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    int fd = open("write.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    } 

    const char* text = "hello, world!";
    printf("%s\n", text);

    size_t to_write = strlen(text) * sizeof(char);
    ssize_t written = write(fd, text, to_write);
    if (written == -1) {
        perror("write");
        return -1;
    }

    printf("expect: %ld, written: %ld\n", to_write, written);
    close(fd);

    return 0;
}
