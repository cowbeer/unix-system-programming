#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    if (argc < 2)
	goto usage;

    int fd = open("nolock.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
	perror("open");
        return -1;
    }

    size_t i, len = strlen(argv[1]);
    for (i = 0; i < len; ++i) {
        if (write(fd, &argv[1][i], sizeof(char)) == -1) {
            perror("write");
	    return -1;
	}
	sleep(1);
    }

    close(fd);

    return 0; 
usage:
    fprintf(stderr, "%s <string>\n", argv[0]);
    return -1;
}
