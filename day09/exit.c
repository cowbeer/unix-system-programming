#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PI 3.14159

void* thread_area(void* arg) {
    double r = *(double*)arg;
    double* s = malloc(sizeof(double));
    *s = PI * r * r;
    pthread_exit(s); // return s;
    *s = 2 * PI * r;
    return s;
}

int main(void) {
    printf("r = ");
    double rs;
    scanf("%lf", &rs);
    pthread_t tid;
    int err = pthread_create(&tid, NULL, thread_area, &rs);
    if (err) {
        fprintf(stderr, "pthread_create: %s\n", strerror(err));
        return -1;
    }

    double* s;
    if ((err = pthread_join(tid, (void**)&s)) != 0) {
        fprintf(stderr, "pthread_join: %s\n", strerror(err));
        return -1;
    }

    printf("s = %g\n", *s);
    free(s);

    return 0;
}

