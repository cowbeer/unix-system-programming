#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

const int const_global = 0;
int init_global = 0;
int uninit_global;


int main(int argc, char* argv[]) {
    const static int const_static = 0;
    static int init_static = 0;
    static int uninit_static;

    const int const_local = 0;
    int prev_local;
    int next_local;

    int* prev_heap = (int*)malloc(sizeof(int));
    int* next_heap = (int*)malloc(sizeof(int));

    const char* literal = "literal";

    extern char** environ;

    printf("---- cmd env ----\n");
    printf("                 cmd: %p\n", argv);
    printf("                 env: %p\n", environ);
    printf("---- stack -----\n");
    printf("         const local: %p\n", &const_local);
    printf("          prev local: %p\n", &prev_local);
    printf("          next local: %p\n", &next_local);
    printf("---- heap  ----\n");
    printf("           next heap: %p\n", next_heap);
    printf("           prev heap: %p\n", prev_heap);
    printf("----- BSS -----\n");
    printf("uninitialized global: %p\n", &uninit_global);
    printf("uninitialized static: %p\n", &uninit_static);
    printf("----- data -----\n");
    printf("  initialized static: %p\n", &init_static);
    printf("  initialized global: %p\n", &init_global);
    printf("---- -text -----\n");
    printf("        const static: %p\n", &const_static);
    printf("             literal: %p\n", literal);
    printf("        const global: %p\n", &const_global);
    printf("            function: %p\n", main);
    printf("/proc/%u/maps", getpid());
    getchar(); 
    return 0;
}
