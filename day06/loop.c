#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void) {
    printf("PID: %d, <Ctrl+C> or <Ctrl+\\>\n", getpid());

    for(;;);

    return 0;
}
