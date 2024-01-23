#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* thread_proc(void* arg) {
    //pthread_detach(pthread_self());

    int i;
    for (i = 0; i < 200; ++i) {
        putchar('-');
        usleep(10000);
    }
    return NULL;
}

int main(void) {
    setbuf(stdout, NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_proc, NULL);
    pthread_detach(tid);
    int err = pthread_join(tid, NULL);
    if (err) {
        fprintf(stderr, "pthread_join: %s\n", strerror(err));
    }
    int i;
    for (i = 0; i < 200; ++i) {
        putchar('+');
        usleep(20000);
    }

    return 0;
}
