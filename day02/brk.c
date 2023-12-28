#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void presskey() {
    printf("/proc/%u/maps", getpid());
    getchar();
}


int main(void) {
    void* p1 = sbrk(4);
    printf("p1 = %p\n", p1);
    
    void* p2 = sbrk(4);
    printf("p2 = %p\n", p2);
    
    void* p3 = sbrk(4);
    printf("p3 = %p\n", p3);

    void* p4 = sbrk(4);
    printf("p4 = %p\n", p4);

    void* p5 = sbrk(0);
    printf("p5 = %p\n", p5);
    
    int* pn = (int*)p1;
    pn[0] = 0;
    pn[1] = 1;
    pn[2] = 2;
    pn[3] = 3;
    pn[1023] = 1023; 
    printf("%d, %d, %d, %d, %d\n", pn[0], pn[1], pn[2], pn[3], pn[1023]);
    void* p6 = sbrk(-8);
    printf("p6 = %p\n", p6);
    void* p7 = sbrk(-8);
    printf("p7 = %p\n", p7);
    pn[0] = 0;


    int page = getpagesize();
    printf("%p\n", sbrk(page));
    presskey();
    printf("%p\n", sbrk(1));
    presskey();
    printf("%p\n", sbrk(-1));
    presskey();
    printf("%p\n", sbrk(-page));
    presskey();
    printf("-------------------------------\n");
    p1 = sbrk(0);
    printf("p1 = %p\n", p1);
    brk(p2 = p1 + 4);
    printf("p2 = %p\n", p2);
    brk(p3 = p2 + 4);
    printf("p3 = %p\n", p3);
    brk(p4 = p3 + 4);
    printf("p4 = %p\n", p4);
    brk(p5 = p4 + 4);
    printf("p5 = %p\n", p5);
    
    
    pn = (int*)p1;
    pn[0] = 0;
    pn[1] = 1;
    pn[2] = 2;
    pn[3] = 3;
    pn[1023] = 1023; 
    printf("%d, %d, %d, %d, %d\n", pn[0], pn[1], pn[2], pn[3], pn[1023]);
    
    brk(p3);
    brk(p1);

    printf("--------------------------------\n");
    void* begin = sbrk(sizeof(int));
    pn = (int*)begin;
    *pn = 1234;
    double* pd = (double*)sbrk(sizeof(double));
    *pd = 3.14;
    char* psz = (char*)sbrk(sizeof(char)*256);
    sprintf(psz, "hello, world!");
    printf("%d, %lf, %s\n", *pn, *pd, psz);
    brk(begin);
    
    return 0;
}
