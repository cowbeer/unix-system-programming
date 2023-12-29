#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv []) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <distfile> <srcfile>\n", argv[0]);
        return -1;
    }

    int src = open(argv[1], O_RDONLY);
    if (src==-1) {
        perror("open");
        return -1;
    }
    
    struct stat st;
    if (fstat(src, &st) == -1) {
        perror("fstat");
	return -1;
    }

    int dest = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, st.st_mode);
    if (dest == -1) {
	if (errno != EEXIST) {
	    perror("open");
	    return -1;
	}
        printf("file %s exists, override? (y/n)", argv[2]);
        int ch = getchar();
	if (ch != 'y' && ch != 'Y') {
	    return 0;
	}

	if ((dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, st.st_mode)) == -1) {
	    perror("error");
	    return -1;
	}
    }

    unsigned char buf[1024];
    ssize_t bytes;
    while ( (bytes = read(src, buf, sizeof(buf))) > 0 ) {
       if ((write(dest, buf, bytes)) == -1) {
           perror("write");
           return -1;
       }		    
    }
    
    if (bytes == -1) {
	perror("read");
        return -1;
    }

    close(dest);
    close(src);
    
    

    return 0;
}
