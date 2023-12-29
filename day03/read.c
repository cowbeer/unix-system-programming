#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(void) {
    int fd = open("read.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    char text[256];
    
    size_t to_read = sizeof(text);
    ssize_t read_len = read(fd, text, to_read);
    if (read_len == -1) {
        perror("read");
	return -1;
    }
    printf("expect: %ld, read: %ld\n", to_read, read_len);
    text[read_len] = '\0';
    printf("content: %s\n", text);

    close(fd);

    return 0;
}
