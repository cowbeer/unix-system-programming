#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <dir>\n", argv[0]);
        return -1;
    }

    DIR* dp = opendir(argv[1]);
    if (!dp) {
        perror("opendir");
        return -1;
    }
    errno = 0;
    struct dirent* de;
    for (de = readdir(dp); de; de = readdir(dp)) {
        switch (de->d_type) {
            case DT_DIR:
                printf("        dir: ");
                break;
            case DT_REG:
                printf("       file: ");
                break;
            case DT_LNK:
                printf("   softlink: ");
                break;
            case DT_BLK:
                printf("  block dev: ");
                break;
            case DT_CHR:
                printf("   char dev: ");
            case DT_SOCK:
                printf("  unix sock: ");
                break;
            case DT_FIFO:
                printf("       fifo: ");
                break;
            default:
                printf("    unknown: ");
        }
        printf("%s\n", de->d_name);
    }
    if (errno) {
        perror("readdir");
        return -1;
    }

    closedir(dp);

    return 0;
}


