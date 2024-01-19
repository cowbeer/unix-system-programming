#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    printf("%d进程: 获得消息队列的键\n", getpid());
    key_t key = ftok(".", 100);
    if (key == -1) {
        perror("ftok");
        return -1;
    }
    printf("%d进程: 消息队列的键: %#x\n", getpid(), key);

    printf("%d进程: 创建消息队列\n", getpid());
    int msgid = msgget(key, 0644|IPC_CREAT|IPC_EXCL);
    if (msgid == -1) {
        perror("shmget");
        return -1;
    }
    printf("%d进程: 消息队列标识: %d\n", getpid(), msgid);

    printf("%d进程: 向消息队列发送数据\n", getpid());
    for (;;) {
        printf("> ");
        struct {
            long type;
            char data[1024];
        } msg = {1234, ""};
        gets(msg.data, sizeof(msg.data)/sizeof(msg.data[0]), stdin); // 会追加\0
        if (!strcmp(msg.data,"!")) {
            break;
        }
        if (msgsnd(msgid, &msg, strlen(msg.data)*sizeof(msg.data[0]), 0) == -1) {
            perror("msgsnd");
            return -1;
        }
    }

    printf("%d进程: 销毁消息队列\n", getpid());
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("shmctrl");
        return -1;
    }
    return 0;
}