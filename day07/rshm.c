#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <time.h>

int shmstat(int shmid) {
    struct shmid_ds shm;
    if (shmctl(shmid, IPC_STAT, &shm) == -1) {
        perror("shmctl");
        return -1;
    }

    printf("key: %#X\n", shm.shm_perm.__key);
    printf("uid: %d\n", shm.shm_perm.uid);
    printf("gid: %d\n", shm.shm_perm.gid);
    printf("cuid: %d\n",shm.shm_perm.cuid);
    printf("cgid: %d\n", shm.shm_perm.cgid);
    printf("mode: %#o\n", shm.shm_perm.mode);
    printf("seq: %d\n", shm.shm_perm.__seq);
    printf("size: %zu\n", shm.shm_segsz);
    printf("last attach time: %s", ctime(&shm.shm_atime));
    printf("last detach time: %s", ctime(&shm.shm_dtime));
    printf("last change time: %s", ctime(&shm.shm_ctime));
    printf("creator PID: %d\n", shm.shm_cpid);
    printf("last loader PID: %d\n", shm.shm_lpid);
    printf("nattach: %lu\n", shm.shm_nattch);

    return 0;
}

int shmset(int shmid) {
    struct shmid_ds shm = {};
    if (shmctl(shmid, IPC_STAT, &shm) == -1) {
        perror("shmctl");
        return -1;
    }

    shm.shm_perm.mode = 0600;
    shm.shm_segsz = 4096 * 2;
    if (shmctl(shmid, IPC_SET, &shm) == -1) {
        perror("shmctl");
        return -1;
    }

    return 0;
}

int main(void) {
    printf("get shm...\n");
    key_t key = ftok(".", 100);
    if (key == -1) {
        perror("ftok");
        return -1;
    }
    int shmid = shmget(key, 0, 0);
    if (shmid == -1) {
        perror("shmget");
        return -1;
    }

    printf("attach shm...\n");
    void* shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1) {
        perror("shmat");
        return -1;
    }

    shmstat(shmid);
    printf("read shm...\n");
    printf("shm(key=0x%08x, id=%d): %s\n", key, shmid, (char*)shmaddr);
    printf("press enter to detach shm...\n");
    getchar();
    printf("detach shm...\n");
    if (shmdt(shmaddr)) {
        perror("shmdt");
        return -1;
    }
    shmstat(shmid);
    printf("ok...\n");
    return 0;
}
