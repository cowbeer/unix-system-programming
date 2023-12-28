#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(void) {
    FILE* fp = fopen("none", "r");
    fp = fopen("/etc/passwd", "r");
    if(errno) {
        perror("fopen");
	printf("fp=%p\n", fp);
	return -1;
    }
    return 0;
}
