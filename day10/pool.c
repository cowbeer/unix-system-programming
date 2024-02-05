#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CONNS 5
#define MAX_USERS 50

sem_t g_sem;
void* thread_user(void* arg) {
    pthread_t tid = pthread_self();

    int semval;
    sem_getvalue(&g_sem, &semval);
    printf("%lu: waiting for connection, remaining(%d)\n", tid, semval);

    sem_wait(&g_sem);

    sem_getvalue(&g_sem, &semval);
    printf("%lu: got connection, remaining(%d)\n", tid, semval);
    usleep(1000000);

    sem_post(&g_sem);

    sem_getvalue(&g_sem, &semval);
    printf("%lu: release connection, remaining(%d)\n", tid, semval);

    return NULL;
}

int main(void) {
    sem_init(&g_sem, 0, MAX_CONNS);

    size_t i;
    pthread_t tids[MAX_USERS];
    int err;
    for (i = 0; i < sizeof(tids)/sizeof(tids[0]); ++i) {
        if ((err = pthread_create(&tids[i], NULL, thread_user, NULL)) != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(err));
            return -1;
        }
    }

    for (i = 0; i < sizeof(tids)/sizeof(tids[0]); ++i) {
        if ((err = pthread_join(tids[i], NULL)) != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(err));
            return -1;
        }
    }

    sem_destroy(&g_sem);

    return 0;
}
