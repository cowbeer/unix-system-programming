#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAX_TEXT 256

int main(void) {
    char* psz = (char*)mmap(NULL, MAX_TEXT * sizeof(char), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (psz == MAP_FAILED) {
        perror("mmap");
	return -1;
    }

    sprintf(psz, "hello, world!");
    printf("%s\n", psz);
    printf("%p\n", psz);
    printf("/proc/%u/maps", getpid());
    getchar();

    if (-1 == munmap(psz, MAX_TEXT * sizeof(char))) {
        perror("munmap");
	return -1;
    }

    return 0;
}
