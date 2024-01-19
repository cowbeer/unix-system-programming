#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/msg.h>

int main(void) {
    printf("get msg queue...\n");
    key_t key = ftok(".", 100);
    if (key == -1) {
        perror("ftok");
        return -1;
    }
    int msqid = msgget(key, 0);
    if (msqid == -1) {
        perror("msgget");
        return -1;
    }
    printf("receiving msg...\n");
    for (;;) {
        struct {
            long mtype;
            char mtext[1024];
        } msgbuf = {};
        ssize_t msgsz = msgrcv(msqid, &msgbuf, sizeof(msgbuf.mtext)-sizeof(msgbuf.mtext[0]), 1234, MSG_NOERROR|IPC_NOWAIT);
        if (msgsz == -1) {
            if (errno == EIDRM) {
                printf("msgq(0x%08X/%d) destroyed...\n", key, msqid);
                break;
            }
            else if (errno == ENOMSG) {
                printf("do other things...\n");
                sleep(1);
            }
            else {
                perror("msgrcv");
                return -1;
            }
        }
        else {
            printf("%04zd< %s\n", msgsz, msgbuf.mtext);
        }
    }
    printf("exit...\n");
    return 0;
}

