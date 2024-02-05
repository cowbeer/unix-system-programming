#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define DINNERS 5

typedef enum tag_Stage {
    THINKING,
    HUNGRY,
    EATING
} ESTAT;

pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_conds[DINNERS] = {PTHREAD_COND_INITIALIZER,
                                   PTHREAD_COND_INITIALIZER,
                                   PTHREAD_COND_INITIALIZER,
                                   PTHREAD_COND_INITIALIZER,
                                   PTHREAD_COND_INITIALIZER};
const char* g_names[DINNERS] = {"董天圣", "董天念", "董天地", "董天人", "董天下"};
ESTAT g_stats[DINNERS] = {THINKING, THINKING, THINKING, THINKING, THINKING};

void eat(int i) {
    int left = (i + 1) % DINNERS;
    int right = (i + DINNERS - 1) % DINNERS;
    pthread_mutex_lock(&g_mtx);
    while(g_stats[left] == EATING || g_stats[right] == EATING) {
        g_stats[i] = HUNGRY;
        printf("%s: hungry...\n", g_names[i]);
        pthread_cond_wait(&g_conds[i], &g_mtx);
    }
    g_stats[i] = EATING;
    printf("%s: eating...\n", g_names[i]);
    pthread_mutex_unlock(&g_mtx);
    usleep((rand() % 100) * 10000);
}

void think(int i) {
    int left = (i + 1) % DINNERS;
    int right = (i + DINNERS - 1) % DINNERS;
    pthread_mutex_lock(&g_mtx);
    g_stats[i] = THINKING;
    printf("%s: thinking..\n", g_names[i]);
    pthread_cond_signal(&g_conds[left]);
    pthread_cond_signal(&g_conds[right]);
    pthread_mutex_unlock(&g_mtx);
    usleep((rand() % 100) * 10000);
}

void* diner(void* arg) {
    int i = (int)arg;

    while (1) {
        eat(i);
        think(i);
    }

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    size_t i;
    pthread_t tid;
    for (i = 0; i < DINNERS; ++i) {
        pthread_create(&tid, &attr, diner, (void*)i);
    }

    getchar();

    return 0;
}
