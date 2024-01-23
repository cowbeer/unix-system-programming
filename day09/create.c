#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* thread_proc(void* arg) {
    printf("Thread(%lu): %s\n", pthread_self(), (char*)arg);
    return NULL;
}

int main(void) {
    pthread_t tid;
    int err = pthread_create(&tid, NULL, thread_proc, "hello");
    if (err) {
        fprintf(stderr, "pthread_create: %s\n", strerror(err));
        return -1;
    }

    printf("MainThread(%lu), ChildThread(%lu)\n", pthread_self(), tid);
    sleep(1);

    return 0;
}
