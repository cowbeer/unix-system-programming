#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

int main(void) {
    printf("create shm...\n");
    key_t key = ftok(".", 100);
    if (key == -1) {
        perror("ftok");
        return -1;
    }

    int shmid = shmget(key, 4096, 0644 | IPC_CREAT | IPC_EXCL);
    if (shmid == -1) {
        perror("shmget");
        return -1;
    },

    printf("attach shm...\n");
    void* shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1) {
        perror("shmat");
        return -1;
    }

    printf("write to shm...\n");
    sprintf(shmaddr,"hello,world!");

    printf("press enter to detach(key=0x%08X,id=%d)...\n", key, shmid);
    getchar();
    if (shmdt(shmaddr) == -1){
        perror("shmaddr");
        return -1;
    }

    printf("press enter to destroy shm...\n");
    getchar();
    if (shmctl(shmid,IPC_RMID, NULL) == -1) {
        perror("shmctl");
        return -1;
    }

    printf("ok...\n");

    return 0;
}
