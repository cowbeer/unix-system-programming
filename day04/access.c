#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return -1;
    }

    printf("file: %s\n", argv[1]);
    if (access(argv[1], F_OK) == -1) {
        printf("file not exists: %m\n");
    }
    else {
        if (access(argv[1], R_OK) == -1) {
            printf("can't read: %m\n");
        }
        else {
            printf("can read\n");
        }

        if (access(argv[1], W_OK) == -1) {
            printf("can't write: %m\n");
        }
        else {
            printf("can write\n");
        }

        if (access(argv[1], X_OK) == -1) {
            printf("can't execute: %m\n");
        }
        else {
            printf("can execute\n");
        }
    }


    return 0;
}
