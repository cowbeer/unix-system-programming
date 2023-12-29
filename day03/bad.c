#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char* text = "file content!!!";
    /*if (write(3, text, strlen(text)) == -1) {
        perror("write");
        return -1;
    }*/
   
    close(1);
    creat("bad.txt", 0644);
    printf("%s\n", text);
    return 0;
}
