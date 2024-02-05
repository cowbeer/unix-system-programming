#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

unsigned int g_cn = 0;
//pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_mtx;

void* thread_add(void* arg) {
    unsigned int i;
    for (i = 0; i < 1000000; ++i) {
        pthread_mutex_lock(&g_mtx);
        ++g_cn;
        pthread_mutex_unlock(&g_mtx);
    }
    return NULL;
}

void* thread_sub(void* arg) {
    unsigned int i;
    for (i = 0; i < 1000000; ++i) {
        pthread_mutex_lock(&g_mtx);
        --g_cn;
        pthread_mutex_unlock(&g_mtx);
    }
    return NULL;
}

int main(void) {
    pthread_mutex_init(&g_mtx, NULL);
    pthread_t tid[2];
    int i;
    pthread_create(&tid[0], NULL, thread_add, NULL);
    pthread_create(&tid[1], NULL, thread_sub, NULL);
    for (i = 0; i < 2; ++i) {
        pthread_join(tid[i], NULL);
    }


    printf("g_cn = %d\n", g_cn);

    return 0;
}