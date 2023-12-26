#include <stdio.h>
#include <dlfcn.h>


typedef int(*PFUNC_CALC)(int, int);
typedef void(*PFUNC_SHOW)(int, char, int, int);

int main(void) {
    void* handle = dlopen("shared/libmath.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        return -1;
    }

    PFUNC_CALC add = (PFUNC_CALC)dlsym(handle, "add");
    if (!add) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
    }
    
    PFUNC_CALC sub = (PFUNC_CALC)dlsym(handle, "sub");
    if (!sub) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
    }

    PFUNC_SHOW show = (PFUNC_SHOW)dlsym(handle, "show");
    if (!show) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
    }
    
    show(30, '+', 20, add(30, 20));
    show(30, '-', 20, sub(30, 20));

    
    if (dlclose(handle)) {
       fprintf(stderr, "dlclose: %s\n", dlerror());
    }


    return 0;
}
