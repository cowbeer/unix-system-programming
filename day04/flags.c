#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void pflags(int flags) {
    printf("file stat flags(%08X):", flags);
    struct{
        int flag;
	const char* desc;
    } flist[] = {
        O_RDONLY, "O_RDONLY",
	O_WRONLY,"O_WRONLY",
	O_RDWR,"O_RDWR",
	O_APPEND,"O_APPEND",
	O_CREAT,"O_CREAT",
	O_EXCL,"O_EXCL",
	O_TRUNC,"O_TRUNC",
	O_NOCTTY,"O_NOCTTY",
	O_NONBLOCK,"O_NONBLOCK",
	O_SYNC, "O_SYNC",
	O_DSYNC, "O_DSYNC",
	O_RSYNC, "O_RSYNC",
	O_ASYNC, "O_ASYNC"
    };

    size_t i;
    int first = 1;
    for (i = 0; i < sizeof(flist)/sizeof(flist[0]); ++i) {
        if (flags & flist[i].flag) {
	    printf("%s%s", first ? "" : "|", flist[i].desc);
	    first = 0;
	}
    }
    printf("\n");
}

int main() {
    printf("O_RDONLY: %d\n", O_RDONLY);
    printf("O_WRONLY: %d\n", O_WRONLY);
    printf("O_RDWR: %d\n", O_RDWR);
    printf("O_APPEND: %d\n", O_APPEND);
    printf("O_CREAT: %d\n", O_CREAT);
    printf("O_EXCL: %d\n", O_EXCL);
    printf("O_TRUCN: %d\n", O_TRUNC);
    printf("O_NOCTTY: %d\n", O_NOCTTY);
    printf("O_NONBLOCK: %d\n", O_NONBLOCK);
    printf("O_SYNC: %d\n", O_SYNC);
    printf("O_DSYNC: %d\n", O_DSYNC);
    printf("O_RSYNC: %d\n", O_RSYNC);
    printf("O_ASYNC: %d\n", O_ASYNC);
    printf("-----------------------------------------\n");

    int fd = open("flags.txt", O_WRONLY | O_CREAT | O_ASYNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }
    printf("flags: %d\n", flags);
    pflags(flags);
    
    if (fcntl(fd, F_SETFL, O_RDWR | O_APPEND | O_NONBLOCK) == -1) {
        perror("fcntl");
        return -1;
    }

    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }
    pflags(flags);
    

    close(fd);
    return 0;
}
