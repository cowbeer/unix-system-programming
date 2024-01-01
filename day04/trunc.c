#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
    const char* text = "hello, world !";
    size_t size = strlen(text) * sizeof(text[0]);
    int fd = open("trunc.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    if (ftruncate(fd, size) == -1) {
        perror("ftruncate");
        return -1;
    }

    void* map = mmap(NULL, size, PROT_READ | PROT_WRITE, /*MAP_PRIVATE*/ MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    memcpy(map, text, size);
    printf("%s\n", (char*)map);
    munmap(map, size);

    close(fd);

    return 0;
}


