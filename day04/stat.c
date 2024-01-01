#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

const char* mtos(mode_t m) {
    static char s[11] = {0};
    if (S_ISDIR(m)) {
        strcpy(s, "d");
    }
    else if (S_ISLNK(m)) {
        strcpy(s, "l");
    }
    else if (S_ISBLK(m)) {
        strcpy(s, "b");
    }
    else if (S_ISCHR(m)) {
        strcpy(s, "c");
    }
    else if (S_ISSOCK(m)) {
        strcpy(s, "s");
    }
    else if (S_ISFIFO(m)) {
        strcpy(s, "p");
    }
    else {
        strcpy(s, "-");
    }

    strcat(s, m & S_IRUSR ? "r" : "-");
    strcat(s, m & S_IWUSR ? "w" : "-");
    strcat(s, m & S_IXUSR ? "x" : "-");
    strcat(s, m & S_IRGRP ? "r" : "-");
    strcat(s, m & S_IWGRP ? "w" : "-");
    strcat(s, m & S_IXGRP ? "x" : "-");
    strcat(s, m & S_IROTH ? "r" : "-");
    strcat(s, m & S_IWOTH ? "w" : "-");
    strcat(s, m & S_IXOTH ? "x" : "-");

    if (m & S_ISUID) {
        s[3] = (s[3] == 'x' ? 's' : 'S');
    }
    if (m & S_ISGID) {
        s[6] = (s[6] == 'x' ? 's' : 'S');
    }
    if (m & S_ISVTX) {
        s[9] = (s[9] == 'x' ? 't' : 'T');
    }

    return s;
}

const char* ttos(time_t t) {
    static char s[20]  = { 0 };
    struct tm* lt = localtime(&t);
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);

    return s;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        goto usage;
    }
    struct stat st;
    if (argc < 3) {
        if (stat(argv[1], &st) == -1) {
            perror("stat");
            return -1;
        }
    }
    else if (!strcmp(argv[2], "-l")) {
        if (lstat(argv[1], &st) == -1) {
            perror("lstat");
            return -1;
        }
    }
    else {
        goto usage;
    }
    printf("                 dev ID: %lu\n", st.st_dev);
    printf("                 i node: %lu\n", st.st_ino);
    printf("                   mode: %s\n", mtos(st.st_mode));
    printf("                  nlink: %lu\n", st.st_nlink);
    printf("                    uid: %u\n", st.st_uid);
    printf("                    gid: %u\n", st.st_gid);
    printf("                   rdev: %lu\n", st.st_rdev);
    printf("                   size: %lu\n", st.st_size);
    printf("             block size: %ld\n", st.st_blksize);
    printf("                 blocks: %ld\n", st.st_blocks);
    printf("       last access time: %s\n", ttos(st.st_atime));
    printf(" last modification time: %s\n", ttos(st.st_mtime));
    printf("last status change time: %s\n", ttos(st.st_ctime));
    return 0;
usage:
    fprintf(stderr, "usage: %s <string> [-l]\n", argv[0]);
    return -1;
}