#include <stdio.h>
#include <limits.h>

long fsize(const char* path) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    fclose(fp);
    return size;
}

int main(void) {
    printf("file path:");
    char path[PATH_MAX+1];
    scanf("%s", path);

    long size = fsize(path);
    if (size<0) {
	printf("failed to get file size.\n");
        return -1;
    }
    printf("file size: %ld\n", size);
    return 0;
}

