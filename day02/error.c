#include <stdio.h>

size_t slen(const char* s) {
    if(!s) {
        return -1;
    }

    size_t len;
    for(len=0; s[len]; ++len);

    return len;
}

char* scpy(char* dst, size_t size, const char* src) {
    if (!dst || !size) {
        return NULL;
    }

    size_t len = slen(src);
    if (len == -1) {
        return NULL;
    }
    
    size_t i, chs = size - 1 < len ? size-1 : len;
    for(i = 0; i < chs; ++i) {
        dst[i] = src[i];
    }
    dst[i] = '\0';

    return dst;
}

int intmin(int a, int b, int* min) {
    if (a==b) {
        return -1;
    }
    *min  = a < b ? a : b;

    return 0;
}

int intavg(int a, int b) {
    //return (a + b) / 2;

    return (a & b) + ((a ^ b) >> 1);
}

int main() {
    size_t len = slen("hello, world");
    if (len == -1) {
        printf("failed to get string length.\n");
    } else {
        printf("string length: %lu\n", len);
    }

    char dst[50];
    if (!scpy(dst, sizeof(dst)/sizeof(dst[0]), "hello,world!")) {
        printf("failed to copy string.\n");
    } else {
        printf("string copy: %s\n", dst);
    }

    int min;
    if (intmin(-1,0, &min) == -1) {
        printf("failed to get min value.\n");
    }else{
        printf("min: %d\n", min);
    }
    
    printf("average: %d\n", intavg(1234, 5678));

    return 0;
}
