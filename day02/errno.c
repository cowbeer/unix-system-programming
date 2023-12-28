#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    FILE* fp = fopen("none", "r");
    if (!fp) {
	printf("error number: %d\n", errno);
	printf("error string: %s\n", strerror(errno));
        perror("fopen");
	printf("fopen: %m\n");
	return -1;
    }
    return 0;
}
