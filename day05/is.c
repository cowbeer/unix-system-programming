#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    printf("parent:");
    int a, b, c;
    scanf("%d%d%d", &a, &b, &c);
    pid_t pid = fork();
    if(pid==-1) {
        perror("fork");
        return -1;
    }
    if(!pid) { //child process
        scanf("%d%d%d", &a, &b, &c);
        printf("child: %d %d %d\n", a, b, c);
        return 0;
    }

    sleep(1);
    printf("parent: %d %d %d\n", a, b, c);

    return 0;
}

