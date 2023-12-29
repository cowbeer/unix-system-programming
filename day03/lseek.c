#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


int main() {
    int fd = open("seek.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    const char* text = "hello, world !";
    if (write(fd, text, strlen(text)*sizeof(char)) == -1) {
        perror("write");
        return -1;
    }

    off_t pos;
    if ((pos = lseek(fd, -7, SEEK_CUR)) == -1) {
        perror("lseek");
        return -1;
    }
    
    printf("cur pos: %ld\n", pos);

    text = "Linux";
    if (write(fd, text, strlen(text)*sizeof(char)) == -1) {
	 perror("write");
         return -1;
    } 

    if (lseek(fd, 8, SEEK_END) == -1) {
        perror("lseek");
        return -1;
    }

    text = "<-there is hole!";
    if (write(fd, text, strlen(text)*sizeof(char)) == -1) {
	perror("write");
        return -1;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        perror("lseek");
        return -1;
    }

    printf("file size: %ld\n", size);

    close(fd);

    return 0;
}
