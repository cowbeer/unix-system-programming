#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

unsigned int g_cn = 0;

void* thread_proc(void* arg) {
    unsigned int i;
    for (i = 0; i < 1000000; ++i) {
        ++g_cn;
    }

    return NULL;
}

int main(void) {
    pthread_t tid[2];
    int i;
    for (i = 0; i < 2; ++i) {
        pthread_create(&tid[i], NULL, thread_proc, NULL);
    }
    for (i = 0; i < 2; ++i) {
        pthread_join(tid[i], NULL);
    }


    printf("g_cn = %d\n", g_cn);

    return 0;
}


