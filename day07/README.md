# 进程通信

## 何为进程间通信

进程间通信（Interprocess Communication，IPC）是指两个或多个进程进行数据交换的过程。

## 进程间通信分类

简单进程间通信：命令行参数、环境变量、信号、文件

传统进程间通信：管道（fifo/pipe）

XSI进程间通信：共享内存、消息队列、信号量

网络进程间通信：套接字

## 管道

管道是Unix系统最古老的进程间通信方式。

历史上的管道通常指半双工管道，只允许数据单向流通。现代系统打都提供全双工管道，数据可以沿管道双向流通。

有名管道（fifo）：基于有名文件（管道文件）的管道通信。

| 步骤 | 进程A    | 函数       | 进程B    | 步骤 |
| ---- | -------- | ---------- | -------- | ---- |
| 1    | 创建管道 | mkfifo     | /        |      |
| 2    | 打开管道 | open       | 打开管道 | 1    |
| 3    | 读写管道 | read/write | 读写管道 | 2    |
| 4    | 关闭管道 | close      | 关闭管道 | 3    |
| 5    | 删除管道 | unlink     | /        |      |

无名管道

适用于父子进程之间的通信

```
#include <unistd.h>
int pipe(int pipefd[2]);
返回值：成功返回0，失败返回-1
```

通过输出参数pipefd返回两个文件描述符，其中pipefd[0]用于读，pipefd[1]用于写。

用法：

* 调用该函数在内核中创建管道文件，并通过其输出参数获得分别用于读和写的两个文件描述符
* 调用fork函数创建子进程
* 写数据进程关闭读端（pipefd[0]），读数据进程关闭写端（pipefd[1]）
* 传输数据
* 父子进程分别关闭自己的文件描述符

## XSI进程间通信

### IPC标识

内核为每个进程间通信维护了一个结构体形式的IPC对象。该对象可通过一个非负整数的IPC标识来引用。与文件描述符不同，IPC标识在使用时会持续加1，当达到最大值时，向0回转。

### IPC键值

IPC标识是IPC对象的内部名称。若多个进程需要在同一个IPC对象上汇合，则必须通过键值作为其外部名称来引用该对象。

* 无论何时，只要创建IPC对象，就必须指定一个键值。
* 键值的数据类型在`sys/types.h`头文件中被定义为`key_t`，其原始类型就是长整型。

### 客户机进程与服务器进程在IPC对象上的三种汇合方式

* 服务器进程可以IPC_PRIVATE为键值创建一个新的IPC对象，并将该IPC对象的标识存放在某处（如文件中），以方便客户机进程读取。
* 在一个公共头文件中，定义一个客户机进程和服务器进程都认可的键值，服务器进程用此键值创建IPC对象，客户机进程用此键值获取该IPC对象。
* 客户机进程和服务器进程事先约定好一个路径和一个项目ID（0-255），二者通过ftok函数将该路径名和项目ID转换为一致的键值。

```
#include <sys/types.h>
#include <sys/ipc.h>
key_t ftok(const char* pathname, int proj_id);

pathname - 一个真实存在的文件或目录的路径名
proj_id - 项目ID仅低8位有效，其值域位[0,255]

返回值：成功返回0，失败返回-1
```

注意：起作用的pathname参数所表示的巨鲸而是pathname字符串本身。因此假设当前目录是`/home/soft01/uc/day07`，则`ftok(".",100)`和`ftok("/home/soft01/uc/day07",100)`的返回值完全相同。

### IPC对象的创建

* 若以IPC_PRIVATE为键值创建IPC对象，则永远创建成功。

* 若所指定的键值在系统范围内未有任何IPC对象相结合，且创建标志包含`IPC_CREAT`位，则创建成功。

* 若所指定的键值在系统范围内已与某个IPC对象相结合，且创建标志包含`IPC_CREAT`和`IPC_EXCL`，则创建失败。

### IPC对象的销毁控制

* IPC_STAT - 获取IPC对象属性

* IPC_SET - 设置IPC对象属性

* IPC_RMID - 删除IPC对象

## 内存共享

### 基本特点

* 两个或更多进程共享一块由系统内核负责维护的内存区域，其地址空间通常被映射到堆或栈之间。

* 无需复制信息，最快的一种IPC机制。

* 需要考虑同步访问的问题。

* 内核为每个共享内存维护一个shmid_ds结构体形式的共享内存对象。

### 常用函数

#### 创建/获取共享内存

```
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflag);

返回值：成功返回共享内存标识，失败返回-1
```

* 该函数已key参数为键值创建共享内存或获取已有的共享内存。
* size参数为共享内存的字节数，建议取内存页字节数（4096）的整数倍。若希望创建共享内存，则必须执行size参数。若只为了获取已有的共享内存，则size参数可取0。
* shmflag：
    * `0` ：获取，不存在即失败
    * `IPC_CREAT`：创建，不存在即创建，已存在就获取，除非
    * `IPC_EXCL`：排斥，已存在即失败。

#### 加载共享内存

```
#include <sys/shm.h>

void* shmat(int shmid, const void* shmaddr, int shmflag);

返回值：成功返回映射地址，失败返回-1
```

* 将shmid参数所标识的共享内存，映射到调用进程的地址空间。
* 可通过shmaddr参数人为指定映射地址，也可将该参数置NULL，由系统自动选择
* shmflag取值：
    * 0 ：以读写方式使用共享内存
    * SHM_RDONLY：以只读方式使用共享内存
    * SHM_RND：只在shmaddr参数非NULL时起作用。表示对该参数向下取内存页的整数倍作为映射地址。

* 内核将该共享内存的加载级数加1

#### 卸载共享内存

```
#include <sys/shm.h>

int shmdt(const void* shmaddr);

返回值：成功返回0，失败返回-1
```

* 从调用进程的地址空间中取消由shmaddr参数所指向的共享内存映射区域。

* 将内核该共享内存的加载计数减1

#### 销毁控制共享内存

```
#include <sys/shm.h>

int shmctl(int shmid, int cmd, struct shmid_ds* buf);

返回值：成功返回0，失败返回-1

struct shmid_ds {
	struct ipc_perm shm_perm;  // 所有者及权限
	size_t          shm_segsz; // 大小（以字节为单位）
	time_t          shm_atime; // 最后加载时间
	time_t          shm_dtime; // 最后卸载时间
	time_t          shm_ctime; // 最后改变时间
	pid_t           shm_cpid; // 创建进程PID
	pid_t           shm_lpid; // 最后加载卸载进程PID
	shmatt_t        shm_nattch; // 当前加载计数
	...
}

struct ipc_perm {
	key_t          __key; // 键值
	uid_t          uid;   // 有效属主ID
	git_t          gid;   // 有效属组ID
	uid_t          cuid;  // 有效创建者ID
	git_t          cgid;  // 有效创建组ID
	unsigned short mode;  // 权限字
	unsigned short __seq; // 序列号
}
```

cmd取值：

* `IPC_STAT`：获取共享内存的属性，通过buf参数输出。

* `IPC_SET`：设置共享内存的属性，通过buf参数输入，仅以下三个属性可设置：
    * `shmid_ds::shm_perm.uid`
    * `shmid_ds::shm_perm.gid`
    * `shmid_ds::shm_perm.mode`

`IPC_RMID`：标记删除共享内存。并非真正删除共享内存，只是做一个删除标记，禁止其被继续加载，但已有加载依然保留。只有当该共享内存的加载计数为0时，才真正被删除。

| 步骤 | 进程A        | 函数   | 进程B        | 步骤 |
| ---- | ------------ | ------ | ------------ | ---- |
| 1    | 创建共享内存 | shmget | 获取共享内存 | 1    |
| 2    | 加载共享内存 | shmat  | 加载共享内存 | 2    |
| 3    | 使用共享内存 | ...    | 使用共享内存 | 3    |
| 4    | 卸载共享内存 | shmdt  | 卸载共享内存 | 4    |
| 5    | 销毁共享内存 | shmctl | /            |      |

## 消息队列

### 基本特点

* 消息队列是一个由系统内核负责存储和管理，并通过消息队列标识引用的数据链表。

* 可以通过`msgget`函数创建一个新的消息队列，或获取一个已有的消息队列。通过`msgsnd`函数向消息队列的尾部追加消息，通过`msgrcv`函数从消息队列的前端提取消息。

* 消息队列中的每个消息单元包含信息数据外，还包含消息类型和数据长度。

* 内核为每个消息队列维护一个`msqid_ds`结构体形式的消息队列对象。

### 常用函数

#### 创建获取消息队列

```
#include <sys/msg.h>

int msgget(key_t key, int msgflg);

返回值：成功返回消息队列标识，失败返回-1
```

msgflg取值：

* `0`：获取，不存在即失败
* `IPC_CREAT`：创建，不存在即创建，已存在即获取，除非
* `IPC_EXCL`：排斥，已存在即失败

#### 向消息队列发送消息

```
#include <sys/msg.h>

int msgsnd(int msqid, const void* msgp, size_t msgsz, int msgflg);

返回值：成功返回0，失败返回-1
```

* `msgp(msg package)`参数指向一个包含消息类型和消息数据的内存块，该内存块的前4字节必须是一个大于0的整数，代表消息类型，其后紧跟消息数据。消息数据的字节长度用`msgsz`参数表示。注意：msgsz参数并不包含消息类型的字节数（4）。

* 若内核中的消息队列缓冲区有足够的空闲空间，则此函数会将消息拷入该缓冲区并立即返回0，表示发送成功，否则此函数会阻塞，直到内核中的消息队列缓冲区有足够的空闲空间为止（比如有消息被接收）。内核缓冲区大小由内核参数决定。

* 若`msgflg`参数包含`IPC_NOWAIT`位，则当内核中的消息队列缓冲区没有足够的空闲空间时，此函数不会阻塞，而是返回-1，errno为`EAGAIN`。

#### 从消息队列接收消息

```
#include <sys/msg.h>

ssize_t msgrcv(int msqid, void* msgp, size_t msgsz, long msgtyp, int msgflg);

返回值：成功返回接收到的消息数据的字节数，失败返回-1
```

* `msgp`参数指向一个包含消息类型和消息数据的内存块，其中消息数据缓冲区的字节大小用`msgsz`参数表示。
* 若所接收到的消息数据字节数大于`msgsz`参数，即消息台城，且`msgflg`参数包含`MSG_NOERROR`位，则该消息被截取`msgsz`字节返回，剩余部分被丢弃。
* 如果`msgflg`参数不包含`MSG_NOERROR`位，消息又太长，则不对该消息做任何处理，直接返回-1，errno为`E2BIG`。
* msgtyp参数表示期望接收哪类消息：
    * `=0`：返回消息队列中的第一条消息
    * `>0`：若`msgflg`参数不包含`MSG_EXCEPT`位，则返回消息队列中第一个类型为`msgtyp`的消息；`msgflg`参数包含`MSG_EXCEPT`位，则返回消息队列中第一个类型不为`msgtyp`的消息。
    * `<0`：返回消息队列中类型小于等于`msgtyp`的绝对值的消息，若有多个，则取类型最小者。

* 若消息队列中有可接收的消息，则此函数会将该消息移出消息队列并立即返回0，表示接收成功。否则此函数会阻塞，直到消息队列中有课接收的消息为止。
* 若`msgflg`参数包含`IPC_NOWAIT`位，则当消息队列中没有任何可接收消息时，此函数不会阻塞，而是返回-1，errno为`ENOMSG`。

#### 销毁控制消息队列

```
#include <sys/msg.h>

int msgctl(int msqid, int cmd, struct msqid_ds* buf);

返回值：成功返回0，失败返回-1

struct msqid_ds {
    struct ipc_perm  msg_perm;   // 权限信息
    time_t           msg_stime;  // 最后发送时间
    time_t           msg_rtime;  // 最后接收时间
    time_t           msg_ctime;  // 最后改变时间
    unsigned long    msg_cbytes; // 消息队列中的字节数
    msgqnum_t        msg_qnum;   // 消息队列中的消息树
    msglen_t         msg_qbytes; // 消息队列能容纳的最大字节数
    pid_t            msg_lspid;  // 最后发送进程PID
    pid_t            msg_lrpid;  // 最后接收进程PID
};

struct ipc_perm {
    key_t           __key;  //
    uid_t           uid;    //
    gid_t           gid;    //
    uid_t           cuid;   //
    gid_t           cgid;   //
    unsigned short  mode;   //
    unsigned short  __seq;  //
};
```

cmd取值：

* `IPC_STAT`：获取共享内存的属性，通过buf参数输出。

* `IPC_SET`：设置共享内存的属性，通过buf参数输入，仅以下四个属性可设置：
    * `msqid_ds::msg_perm.uid`
    * `msqid_ds::msg_perm.gid`
    * `msqid_ds::msg_perm.mode`
    * `msqid_ds::msg_qbytes`

* `IPC_RMID`：立即删除消息队列。此时所有阻塞在该消息队列的`msgsnd`和`msgrcv`函数调用都会立即返回失败，errno为`EIDRM`。

#### 编程模型

| 编程模式 | 进程A            | 函数          | 进程B            | 步骤 |
| -------- | ---------------- | ------------- | ---------------- | ---- |
| 1        | 获得消息队列的键 | ftok          | 获得消息队列的键 | 1    |
| 2        | 创建消息队列     | msgget        | 获得消息队列     | 2    |
| 3        | 发送接收消息     | msgsnd/msgrcv | 接收发送消息     | 3    |
| 4        | 销毁消息队列     | msgctl        | /                |      |

## 信号量

### 基本特点

* 计数器，用于限制多个进程对有限共享资源的访问。

* 多个进程获取有限共享资源的操作模式
    * A测试控制该资源的信号量
    * B若信号量大于0，则进程可以使用该资源，为了表示此进程已获得该资源，需要将信号量减1
    * C若信号量等于0，则进程休眠等待该资源，直到信号量大于0，进程被唤醒，执行步骤A
    * 当某进程不再使用该资源时，信号量加1，正在休眠等待该资源的其他进程将被唤醒

### 常用函数

#### 创建/获取信号量

```
#include <sys/sem.h>

int semget(key_t key, int nsems, int semflg);

返回值：成功返回信号量集合标识，失败返回-1
```

* 该函数以key参数为键值创建一个信号量集合（nsems参数表示集合中的信号量数），或获取已有的信号量集合（nsems取0）

* semflg取值：
    * `0`：获取，不存在即失败
    * `IPC_CREAT`：创建，不存在即创建，已存在即获取，除非
    * `IPC_EXCL`：排斥，已存在即失败

#### 操作信号量

```
#include <sys/sem.h>

int semop(int semid, struct sembuf* sops, unsigned nsops);

返回值：成功返回0，失败返回-1

struct sembuf {
	unsigned short sem_num; // 信号量下标
	short          sem_op;  // 操作数
	short          sem_flg; // 操作标记
};
```

该函数对semid参数所标识的信号量集合中，由sops参数所指向的包含nsops个元素的，结构体数组中的每个元素，依次执行如下操作：

若`sem_op`大于0，则将其加到第`sem_num`个信号量的计数值上，以表示对资源的释放。

若`sem_op小`于0，则从第`sem_num`个信号量的计数值中减去其绝对值，以表示对资源的获取。

若第`sem_num`个信号量的计数值不够减（信号量不能为负），则此函数会阻塞，直到该信号量够减为止，以表示对资源的等待。

若`sem_flg`包含`IPC_NOWAIT`位，则当地`sem_num`个信号量的计数值不够减时，此函数不会阻塞，而是返回-1，`errno`为`EAGAIN`，以便在等待资源的同时还可以做其他处理。

若`sem_op`等于0，则直到第`sem_num`个信号量的计数值为0时才返回。

#### 删除信号量

```
#include <sys/sem.h>

int semctl(int semid, int semnum, int cmd);
int semctl(int semid, int semnum, int cmd, union semun arg);

返回值：成功返回值因cmd而异，失败返回-1

union semun {
	int               val;   //
	struct semid_ds*  buf;   //
	unsigned short*   array; //
	struct seminfo*   __buf; // 
};

struct semid_ds {
	struct ipc_perm  sem_perm;
	timt_t           sem_otime;
	time_t           sem_ctime;
	unsigned short   sem_nsems;
};

struct ipc_perm {
	key_t          __key; // 键值
	uid_t          uid;   // 有效属主ID
	gid_t          gid;   // 有效数组ID
	uid_t          cuid;  // 有效创建者ID
	gid_t          cgid;  // 有效创建组ID
	unsigned short mode;  // 权限字
	unsigned short __seq; // 序列号
};
```

cmd取值：

* `IPC_STAT`：获取信号量集合的属性，通过arg.buf输出

* `IPC_SET`：设置信号量集合的属性，通过arg.buf，仅以下三个属性可设置：

    * semid_ds::sem_perm.uid

    * semid_ds::sem_perm.gid

    * semid_ds::sem_perm.mode

* `IPC_RMID`：立即删除信号量集合。此时所有阻塞在对该信号量集合的semop函数调用，都会立即返回失败，errno为EIDRM。

* `GETALL`：获取信号量集合中每个信号量的计数值，通过arg.array输出。

* `SETALL`：设置信号量集合总每个信号量的计数值，通过arg.array输入。

* `GETVA`L：获取信号量集合中，第semnum个信号量的计数值，通过返回值输出。

* `SETVAL`：设置信号量集合中，第semnum个信号量的计数值，通过arg.val输入。

注意：只有针对信号量集合中具体某个信号量的操作才会使用semnum参数。针对整个信号量集合的操作会忽略semnum参数。

#### 编程模型

| 步骤 | 进程A      | 函数   | 进程B      | 步骤 |
| ---- | ---------- | ------ | ---------- | ---- |
| 1    | 创建信号量 | semget | 获取信号量 | 1    |
| 2    | 初始信号量 | semctl | /          |      |
| 3    | 加减信号量 | semop  | 加减信号量 | 2    |
| 4    | 销毁信号量 | semctl | /          |      |

#### 显示

```
ipcs -m 显示共享内存
ipcs -q 显示消息队列
ipcs -s 显示信号量
ipcs -a 显示所有IPC对象
```

#### 删除

```
ipcrm -m ID 删除共享内存
ipcrm -q ID 删除消息队列
ipcrm -s ID 删除信号量
```

