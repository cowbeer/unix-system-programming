#include <stdio.h>
#include <string.h>

const char* strmax(const char* a, const char* b) {
    return a && b ? (strcmp(a, b) > 0 ? a : b) : NULL;
}

int main(void) {
    const char* max = strmax("hello", "world");
    if (!max) {
        printf("failed to get the max string.\n");
	return -1;
    }
    printf("max string: %s\n", max);
    return 0;
}
