#include <stdio.h>
#include <string.h>
#include <pthread.h>

void elapse(void) {
    size_t i;
    for (i = 0; i < 800000000; ++i);
}

void* thread_proc(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1) {
        printf("thread: elapse...\n");
        elapse();
    }
    return NULL;
}

int main(void) {
    setbuf(stdout, NULL);
    printf("press enter to cancel thread...\n");
    pthread_t tid;
    pthread_create(&tid, NULL, thread_proc, NULL);
    getchar();
    pthread_cancel(tid);
    printf("sent cancel request, waiting for cancel...\n");
    pthread_join(tid, NULL);
    printf("thread canceled.\n");

    return 0;
}

