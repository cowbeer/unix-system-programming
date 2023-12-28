#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void presskey() {
    printf("/proc/%u/maps, enter to quit", getpid());
    getchar();
}

int main(void) {
    printf("page_size=%d\n", getpagesize()); 
    char* pc = malloc(sizeof(char));
    printf("pc= %p\n", pc);
    presskey();
    free(pc);
    printf("free(%p)\n", pc);
    presskey();
    setbuf(stdout, NULL);
    size_t i = 0;
    for(;;) {
	printf("%ld-->%p, ", i, &pc[i]);
        printf("%c\n", pc[i++] = (i%26+'A'));
    }
    return 0;
}
