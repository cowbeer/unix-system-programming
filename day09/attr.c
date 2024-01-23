#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int print_attr(pthread_attr_t* attr) {
    printf("-----------Thread Attribute-----------\n");
    int detach_state;
    pthread_attr_getdetachstate(attr, &detach_state);
    printf("detach state: %s\n", (detach_state == PTHREAD_CREATE_DETACHED) ? "detached" : (detach_state == PTHREAD_CREATE_JOINABLE) ? "joinable" : "unknown");

    int scope;
    pthread_attr_getscope(attr, &scope);
    printf("scope: %s\n", (scope == PTHREAD_SCOPE_SYSTEM) ? "system" : (scope == PTHREAD_SCOPE_PROCESS) ? "process" : "unknown");

    int inherit;
    pthread_attr_getinheritsched(attr, &inherit);
    printf("inherit schedule: %s\n", (inherit == PTHREAD_INHERIT_SCHED) ? "inherit" : (inherit == PTHREAD_EXPLICIT_SCHED) ? "explicit" : "unknown");

    int policy;
    pthread_attr_getschedpolicy(attr, &policy);
    printf("schedule policy: %s\n", (policy == SCHED_OTHER) ? "other": (policy == SCHED_FIFO) ? "FIFO" : (policy == SCHED_RR) ? "RR" : "unknown");

    struct sched_param param;
    pthread_attr_getschedparam(attr, &param);
    printf("priority: %d\n", param.sched_priority);

    size_t guard_size;
    pthread_attr_getguardsize(attr, &guard_size);
    printf("guard size: %zu\n", guard_size);

    void* stack_addr;
    size_t stack_size;
    pthread_attr_getstack(attr, &stack_addr, &stack_size);
    printf("stack addr: %p\n", stack_addr);
    printf("stack size: %zu\n", stack_size);
    printf("--------------------------------------\n");
    return 0;
}

void* thread_proc(void* arg) {
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);
    print_attr(&attr);
    return NULL;
}

int main(void) {
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);
    print_attr(&attr);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    void* stack_addr = malloc(4096*10);
    printf("stack addr = %p\n", stack_addr);
    pthread_attr_setstack(&attr, stack_addr, 4096*10);

    pthread_t tid;
    pthread_create(&tid, &attr, thread_proc, NULL);

    pthread_attr_destroy(&attr);

    getchar();

    return 0;
}



