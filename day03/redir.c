#include <stdio.h>

int main(void) {
    int data;
    fscanf(stdin, "%d", &data);
    // stdout: has buffer
    fprintf(stdout, "%d\n", data);
    // stderr: has no buffer
    fprintf(stderr, "%d\n", data);


    return 0;
}
