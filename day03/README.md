## 创建虚拟内存到物理内存或文件的映射

```
#include <sys/mman.h>
void* mmap(
	void* start, // 映射区内存起始地址，NULL系统自动选定，填NULL时由系统确定为当前堆尾，即sbrk(0)的返回值
	size_t length, // 字节长度，自动按页（4K）对齐
	int prot, // 映射权限
	int flags, // 映射标志
	int fd, // 文件描述符
	off_t offset // 文件偏移量，自动按页（4K）对齐
);
prot取值：
	PROT_EXEC - 映射区可执行
	PROT_READ - 映射区可读取
	PROT_WRITE - 映射区可写入
	PROT_NONE - 映射区不可访问
flags取值：
	MAP_FIXED - 若在start上无法创建映射，则失败（无此标志系统会自动调整）。
	MAP_SHARED - 对映射区的写入操作直接反应到文件中。
	MAP_PRIVATE -  对映射区的写入操作只反映到缓冲区中，不会真正写入文件。
	MAP_ANONYMOUS - 匿名映射，将虚拟地址映射到物理内存中而非文件，忽略fd。
	MAP_DENYWRITE - 拒绝其他对文件的写入操作。
	MAP_LOCKED - 锁定映射区域，保证其不被置换。
返回值：
	成功返回内存起始地址，失败返回MAP_FAILED(-1)
```

销毁虚拟内存到物理内存或文件的映射

```
int munmap(
	void* start, // 映射区内存起始地址
	size_t length // 字节长度，自动按页（4K）对齐
);
成功返回0，失败返回-1。
```

mmap/munmap底层不会维护任何东西，只是返回一个首地址，所分配内存位于堆中。

brk/sbrk底层维护一个指针，记录所分配的内存结尾，所分配内存位于堆中，底层调用mmap/munmap。

malloc底层维护一个双向链表和必要的控制信息，不可越界访问，所分配内存位于堆中，底层调用brk/sbrk。

每个进程都有4G的虚拟内存空间，虚拟内存地址只是一个数字，并没有和实际物理内存直接关联。所谓内存分配与释放，其本质就是建立或取消虚拟内存和物理内存之间的映射关系。

## 系统调用

内核提供给外界访问的接口函数，调用这些函数将使进程进入内核态。

* Unix/Linux大部分系统功能都是通过系统调用实现，如open/close。

* Unix/Linux的系统调用已经被封装成C函数的形式，但它并不是标准C的一部分。

* 标库库函数大部分时间运行在用户态，但部分函数偶尔也会调用系统调用，进入内核态，如malloc/free。

* 程序员自己写的代码可以调用系统调用，与操作系统内核交互，进入内核态，如brk/sbrk/mmap/munmap。

* 系统调用在内核中实现，其外部接口定义在C库中。该接口的实现借助软中断进入内核。

time命令：测试运行时间

​	real：总执行时间

​	user：用户空间执行时间

​	sys：内核空间执行时间

strace命令：跟踪系统调用

## 一切皆文件

* Linux环境中的文件具有特别重要的意义，因为它为操作系统服务和设备提供了一个简单而统一的接口。在Linux中，（几乎）一切皆文件。

* 程序员完全可以像访问普通磁盘文件一样。

* 大多数情况下只需要使用五个基本系统调用：open/close/read/write/ioctl

* Linux中的任何对象都可以被视为某种特定类型的文件，可以访问文件的方式访问之。

* 广义的文件。
  * 目录文件
  * 设备文件
    * 控制台：/dev/console
    * 声卡：/dev/audio
    * 标准输入输出：/dev/tty
    * 空文件：/dev/null

## 文件相关的系统调用

open - 打开/创建文件

creat - 创建空文件

close - 关闭文件

read - 读取文件

write - 写入文件

lseek - 设置读写位置

fcntl - 修改文件属性

unlink - 删除硬链接

rmdir - 删除空目录

remove - 删除硬链接或空目录

unlink/remove删除的是文件最后一个硬链接并且没有进程正在打开该文件，那么该文件在磁盘的存储区域将被立即标记为自由。反之，如果有进程正在打开该文件，那么该文件在磁盘上的存储区域，将在所有进程关闭该文件之后被标记为自由。

如果被unlink/remove删除的是一个软链接文件，那么仅软链接文件本身被删除，其目标不受影响。软链接是一个存放了另一个文件的路径（硬链接）的文件。

## 文件描述符

* 非负整数
* 表示一个打开的文件
* 由系统调用（open）返回，被内核空间（后续系统调用）引用。
* 内核缺省为每个进程打开三个文件描述符：
  * 0 - 标准输入
  * 1 - 标准输出
  * 2 - 标准错误

在unistd.h中被定义为如下三个宏：

```
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
```

* 文件描述符的范围介于0到OPEN_MAX之间，传统Unix中OPEN_MAX宏被定义为63，线代Linux使用更大的上限。

```
#include <unistd.h>

//创建/读写文件
int open(
	const char* pathname, // 路径
	int flags, // 模式
	mode_t mode //权限（仅创建文件有效）
);

flags为以下值的位或：
    O_RDONLY   - 只读
    O_WRONLY   - 只写
    O_RDWR     - 读写
    O_APPEND   - 追加
    O_CREAT    - 创建，不存在就创建（已存在就直接打开，并保留原内容，除非）
    O_EXCL     - 排斥，已存在就失败
    O_TRUNC    - 清空，已存在就清空
    O_NOCTTY   - 非控，若pathname指向控制终端，则不将该终端作为控制终端
    O_NONBLOCK - 非阻，若pathname指向FIFO/块/字符文件，则该文件的打开及后续操作均为非阻塞模式
    O_SYNC     - 同步，write等待数据和属性，被物理地写入底层硬件后再返回
    O_DSYNC    - 数同，write等待数据，被物理地写入底层硬件后再返回
    O_RSYNC    - 读同，read等待对所访问区域的所有写操作全部完成后再读取并返回
    O_ASYNC    - 异步，当文件描述符可读/写时，像调用进程发送SIGIO信号
    
// 创建文件
int creat(
	const char* pathname, // 路径
	mode_t mode // 权限
);

#include <unistd.h>
int close(
    int fd // 文件描述符
);
成功返回0，失败返回-1
```

open/creat所返回的一定是当前未被使用的最小文件描述符。

一个进程可以同时打开的文件描述符个数受limits.h中定义的OPEN_MAX宏限制，POSIX要求不低于16，传统Unix是63，现代Linux是256。

操作系统可以通过权限掩码（当前为0002）屏蔽程序所创建文件的某些权限位，如：

0666(rw-rw-rw-) & ~0002 = 0664

creat函数是通过调用open实现的。

```
int creat(const char* pathname, mode_t mode) {
	return open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
```

## 写文件

```
#include <unistd.h>
ssize_t write(
	int fd, // 文件描述符
	const void* buf, // 缓冲区
	size_t count // 期望写入的字节数
);
成功返回实际写入的字节数（0表示未写入），失败返回-1。
```

size_t：unsigned int，无符号整数

ssize_t：int，有符号整数

## 读文件

```
#include <unistd.h>

ssize_t read(
	int fd, // 文件描述符
	void *buf, // 缓冲区
	size_t count // 期望读取的字节数
);

成功返回实际读取的字节数（0表示读到文件尾），失败返回-1
```

## 系统IO与标准IO

当系统调用函数被执行时，需要切换用户态和内核态，频发调用会导致性能损失。

标准库做了必要的优化，内部维护了一个缓冲区，只在满足特定条件时才将缓冲区与系统内核同步，借此降低执行系统调用的频率，减少进程在用户态和内核态之间来回切换的次数，提高运行性能。

## lseek

每个打开的文件都有一个与其相关的文件位置。

文件位置通常是一个非负整数，用以度量从文件头开始计算的字节数。

读写操作都从当前文件位置开始，并根据所读写的字节数，增加文件位置。

打开一个文件时，除非指定了O_APPEND，否则文件位置一律被设为0。

lseek函数仅将文件位置记录在内核中，并不引发任何IO动作。

在超越文件尾的文件位置写入数据，将在文件中形成空洞。

文件空洞不占用磁盘空间，但被算在文件大小内。

```
#include <sys/types.h>
#include <unistd.h>

off_t lseek(
	int fd, //文件描述符
	off_t offset, //偏移量
	int whence // 起始位置
);

成功返回当前文件位置，失败返回-1

whence取值：
	SEEK_SET - 文件头（文件的第一个字节）
	SEEK_CUR - 当前位置（上次读写的最后一个字节的下一个位置）
	SEEK_END - 文件尾（文件最后一个字节的下一个位置）
```

## 打开文件的内核数据结构

通过ls -i可以查看文件的i节点号。i节点记录了文件的属性和数据在磁盘上的存储位置。

目录也是文件，存放路径和i节点的映射表。

## dup/dup2

```
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);

成功返回文件描述符oldfd的副本，失败返回-1
```

* 复制一个已经打开的文件描述符
* 返回的一定是当前未被使用的最小文件描述符
* dup2可由第二个参数指定文件描述符的值。若指定的描述符已经打开，则先关闭之。
* 所返回的文件描述符副本与源文件描述符对应同一个文件表。

注意区分通过dup获得的文件描述符副本和两次open同一个文件的区别：

dup只是复制文件描述符，不复制文件表。

```
fd1 \
     ---> 文件表 ---> v节点 ---> i节点
fd2 /
```

open创建新文件表，并为其分配新文件描述符。

```
fd1 ---> 文件表1 \
                 ---> v节点 ---> i节点
fd2 ---> 文件表2 /
```


