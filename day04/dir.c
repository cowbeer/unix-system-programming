#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>


int main() {
    char cwd[PATH_MAX+1] = { 0 };
    if (! getcwd(cwd, sizeof(cwd)/sizeof(cwd[0]))) {
        perror("getcwd");
        return -1;
    }
    printf("cwd: %s\n", cwd);

    if (mkdir("work", 0755) == -1) {
        perror("mkdir");
        return -1;
    }
    if (chdir("work") == -1) {
        perror("chdir");
        return -1;
    }

    if (! getcwd(cwd, sizeof(cwd)/sizeof(cwd[0]))) {
        perror("getcwd");
        return -1;
    }
    printf("cwd: %s\n", cwd);

    if (mkdir("empty", 0755) == -1) {
        perror("mkdir");
        return -1;
    }
    if (rmdir("empty") == -1) {
        perror("rmdir");
        return -1;
    }
    if (rmdir("../work") == -1) {
        perror("rmdir");
        return -1;
    }



    return 0;
}

