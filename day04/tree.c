#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

int tree(const char* dir, int depth) {
    DIR* dp = opendir(dir);
    if (!dp) {
        perror("opendir");
        return -1;
    }

    if (chdir(dir) == -1) {
        perror("chdir");
        return -1;
    }

    errno = 0;
    struct dirent* de;
    for (de = readdir(dp); de; de = readdir(dp)) {
        if (de->d_type != DT_DIR) {
            printf("%*s%s\n", depth*2, "", de->d_name);
        }
        else if (strcmp(de->d_name, ".") && strcmp(de->d_name, "..")) {
            printf("%*s%s/\n", depth*2, "", de->d_name);
            if (tree(de->d_name, depth+1) == -1) {
                return -1;
            }
        }
    }

    if (errno) {
        perror("readdir");
        return -1;
    }

    if(chdir("..") == -1) {
        perror("chdir");
        return -1;
    }

    closedir(dp);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <dir>\n", argv[0]);
        return -1;
    }

    return tree(argv[1], 0);
}