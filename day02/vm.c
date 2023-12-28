#include <stdio.h>
#include <stdlib.h>

int g_vm = 0;

int main(void) {
    printf("&g_vm = %p\n", &g_vm);
    printf("input an integer:");
    scanf("%d%*c", &g_vm);
    
    printf("start a new process, input a different integer:");
    getchar();
    printf("g_vm = %d\n", g_vm);

    return 0;
}
