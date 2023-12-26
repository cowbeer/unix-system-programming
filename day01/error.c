#include <stdio.h>

#if(VERSION < 3)
    #error "version too low"
#elif (VERSION > 3)
   #warning "version too high"
#endif

int main() {
    printf("version: %d\n", VERSION);
    return 0;
}
