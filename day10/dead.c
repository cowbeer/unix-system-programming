#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t g_mtxa = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_mtxb = PTHREAD_MUTEX_INITIALIZER;

void* thread_proc1(void* arg) {
    pthread_t tid = pthread_self();

    printf("%lu: wait A\n", tid);
    pthread_mutex_lock(&g_mtxa);
    printf("%lu: got A\n", tid);
    sleep(1);
    printf("%lu: wait B\n", tid);
    pthread_mutex_lock(&g_mtxb);
    printf("%lu: got B\n", tid);
    //...
    printf("%lu: release b\n", tid);
    pthread_mutex_unlock(&g_mtxb);
    printf("%lu: release a\n", tid);
    pthread_mutex_unlock(&g_mtxa);

    return NULL;
}

void* thread_proc2(void* arg) {
    pthread_t tid = pthread_self();

    printf("%lu: wait B\n", tid);
    pthread_mutex_lock(&g_mtxb);
    printf("%lu: got B\n", tid);
    sleep(1);
    printf("%lu: wait A\n", tid);
    pthread_mutex_lock(&g_mtxa);
    printf("%lu: got A\n", tid);
    //...
    printf("%lu: release a\n", tid);
    pthread_mutex_unlock(&g_mtxa);
    printf("%lu: release b\n", tid);
    pthread_mutex_unlock(&g_mtxb);

    return NULL;
}

int main(void) {
    pthread_t tid1,tid2;
    pthread_create(&tid1, NULL, thread_proc1, NULL);
    pthread_create(&tid2, NULL, thread_proc2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
