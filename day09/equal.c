#include <stdio.h>
#include <pthread.h>
#include <string.h>

pthread_t gmain;

void* ismain(void* arg) {
    //if (pthread_self() == gmain) {
    if (pthread_equal(pthread_self(),gmain)) {
        printf("main thread\n");
    }
    else {
        printf("child thread\n");
    }

    return NULL;
}

int main(void) {
    gmain = pthread_self();
    ismain(NULL);
    pthread_t tid;
    int err = pthread_create(&tid, NULL, ismain, NULL);
    if (err) {
        fprintf(stderr, "pthread_create: %s\n", strerror(err));
        return -1;
    }
    if ((err = pthread_join(tid, NULL)) != 0) {
        fprintf(stderr, "pthread_join: %s\n", strerror(err));
        return -1;
    }
    return 0;
}
