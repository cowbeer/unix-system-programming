#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_exit1(void) {
    printf("do_exit1()...\n");
}

void do_exit2(int status, void* arg) {
    printf("do_exit2(%d, %s)...\n", status, (char*)arg);
}

int foo(void) {
    printf("foo()...\n");

    //exit(EXIT_FAILURE);
    //exit(EXIT_SUCCESS);
    exit(1234);

    //_exit(1234);
    //_Exit(1234);

    return 10;
}

int main(void) {
    if (atexit(do_exit1)) {
        perror("atexit");
        return -1;
    }
    if (on_exit(do_exit2, "goodbye")) {
        perror("on_exit");
        return -1;
    }

    printf("foo() return: %d\n", foo());

    return 1234; //exit(1234);
}