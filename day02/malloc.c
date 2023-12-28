#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// memory control block
typedef struct mem_control_block {
    bool free;
    struct mem_control_block* prev;
    size_t size;
} MCB;

MCB* g_top = NULL;

void* my_malloc(size_t size) {
    MCB* mcb;
    for (mcb = g_top; mcb; mcb = mcb->prev) {
        if (mcb->free && mcb->size >= size) {
	    break;
	}
    }

    if (!mcb) {
        mcb = sbrk(sizeof(MCB) + size);
        if (mcb == (void*)-1) {
	    return NULL;
	}

	mcb->prev = g_top;
	mcb->size = size;

	g_top = mcb;
    }

    mcb->free = false;

    return mcb + 1;
}


void my_free(void* ptr) {
    if (!ptr) {
        return;
    }

    MCB* mcb = (MCB*)ptr - 1;
    mcb->free = true;
    
    for (mcb = g_top; mcb->prev; mcb = mcb->prev) {
        if (!mcb->free) {
	    break;
	}
    }

    if (mcb->free) {
        g_top = mcb->prev;
        brk(mcb);
    } else {
       g_top = mcb;
       brk((void*)mcb + sizeof(mcb) + mcb->size);
    }
}

int main(void) {
    int* p1 = (int*)my_malloc(10 * sizeof(int));
    int* p2 = (int*)my_malloc(5 * sizeof(int));

    for(size_t i = 0; i<10; i++) {
        p1[i] = i + 1;
    }
    for(size_t i = 0; i < 5; i++) {
        p2[i] = 10 + i + 1;
    }
    
    
    for(size_t i = 0; i<10; i++) {
        printf("%d ", p1[i]);
    }
    printf("\n");
    my_free(p1);

    for(size_t i = 0; i < 5; i++) {
        printf("%d ", p2[i]);
    }
    printf("\n");
    my_free(p2);

    return 0;
}

