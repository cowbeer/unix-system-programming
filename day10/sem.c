#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

unsigned int g_cn = 0;

sem_t g_sem;

void* thread_add(void* arg) {
    unsigned int i;
    for (i = 0; i < 1000000; ++i) {
        sem_wait(&g_sem);
        ++g_cn;
        sem_post(&g_sem);
    }
    return NULL;
}

void* thread_sub(void* arg) {
    unsigned int i;
    for (i = 0; i < 1000000; ++i) {
        sem_wait(&g_sem);
        ++g_cn;
        sem_post(&g_sem);
    }
    return NULL;
}

int main(void) {
    sem_init(&g_sem, 0, 1);

    pthread_t tid[2];
    int i;
    pthread_create(&tid[0], NULL, thread_add, NULL);
    pthread_create(&tid[1], NULL, thread_sub, NULL);
    for (i = 0; i < 2; ++i) {
        pthread_join(tid[i], NULL);
    }

    printf("g_cn = %d\n", g_cn);

    sem_destroy(&g_sem);

    return 0;
}