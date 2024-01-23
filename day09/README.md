# 线程管理

## 基本概念

* 线程就是程序的执行路线，即进程内部的控制序列，或者说是进程的子任务。

* 现成，轻量级，不拥有自己独立的内存资源，共享进程的代码区、数据区、堆区（注意没有栈区）、环境变量和命令行参数、文件描述符、信号处理函数、当前目录、用户ID和组ID等资源。

* 线程拥有自己独立的栈，因此也有自己独立的局部变量。

* 一个进程可以同时有多个线程，即同时被系统调度的多条执行路线，但至少要有一个主线程。

## 基本特点

* 线程是进程的一个实体，可作为系统独立调度和分派的基本单位。

* 线程有不同的状态，系统提供了多种线程控制原语，

* 线程不拥有自己的资源，只拥有从属于进程的全部资源，所有的资源分配都是面向进程的。

* 一个进程中可以有多个线程并发地运行。它们可以执行相同的代码，也可以执行不同的代码。

* 同一个进程的多个线程都在同一个地址空间内活动，因此相对于进程，线程的系统开销小，任务切换快。

* 线程间的数据交换不需要依赖于类似IPC的特殊通信机制，简单而高效。

* 每个线程拥有自己独立的线程ID、寄存器信号、函数栈、错误码和信号掩码。

* 线程之间存在优先级的差异。

## POSIX线程（pthread）

* 早期厂商各自提供私有的线程版本库，接口和实现的差异非常大，不易于移植。

* IEEE POSIX 1003.1c（1995）标准，定义了统一的线程编程接口，遵循改标准的线程实现被统称为POSIX线程，即pthread。

* pthread包含一个头文件`pthread.h`和一个接口库`libpthread.so`

```
#include <pthread.h>
gcc ... -lpthread
```

* 功能

    * 线程管理：创建/销毁线程、分离/联合线程、设置/查询线程属性。

    * 线程同步
        * 互斥量：创建/销毁互斥量、加锁/解锁互斥量、设置/查询互斥量属性
        * 条件变量：创建/销毁条件变量、等待/触发条件变量、设置/查询条件变量属性

## 线程函数

### 创建线程

```
#include <pthread.h>

int pthread_create(
    pthread_t* restrict thread, 
    const phtread_t* restrict attr, 
    void* (*start_routine) (void*), 
    void* restrict arg
);

返回值：成功返回0，失败返回错误码
```

* `thread`：线程ID输出参数。`pthread_t`即`unsigned long int`

* `attr`：线程属性，NULL表示缺省属性。`pthread_attr_t`可能是整型也可能是结构，因实现而异。

* `start_routine`：线程过程函数指针，参数和返回值的类型都是`void*`。启动线程本质上就是调用一个函数，只不过是在一个独立的线程中调用的，函数返回即线程结束。

* `arg`：传递给线程过程函数的参数。线程过程函数的调用者是系统内核，而非用户代码，因此需要在创建线程时指定参数。

注意：

* `restrict`：C99引入的编译优化指示符，提高重复解引用同一个指针的效率。
* 在`pthread.h`头文件中声明的函数通常以直接返回错误码的方式表示失败，而非以错误码设置errno并返回-1。
* main函数即主线程，main函数返回即主线程结束，主线程结束即进程结束，进程一旦结束其所有的线程即结束。
* 应设法保证在线程过程函数执行期间，其参数所指向的目标持久有效。

### 等待线程结束

```
#include <pthread.h>

int thread_join(pthread_t thread, void** retval);

返回值：成功返回0，失败返回错误码
```

注意从线程过程函数中返回值的方法：

* 线程过程函数将所需返回的内容放在一块内存中，返回该内存的地址，保证这块内存在函数返回即线程结束以后依然有效。
* 若`retval`参数非`NULL`，则`pthread_join`函数将线程过程函数所返回的指针拷贝到该参数所指向的内存中。
* 若线程过程函数所返回的指针指向动态分配的内存，则还需保证在用过该内存之后释放之。

### 获取线程自身ID

```
#include <pthread.h>

pthread_t pthread_self(void);

返回值：返回调用线程的ID，不会失败
```

### 比较两个线程ID

```
#include <pthread.h>

int pthread_equal(pthread_t t1, pthread_t t2);

返回值：若参数t1和t2所标识的线程ID相等，则返回非0，否则返回0
```

某些实现的`pthread_t`不是`unsigned long int`类型，可能是结构体类型，无法通过`==`判断其相等性。

### 终止线程

* 从线程调用函数中return
* 调用`pthread_exit`函数

```
#include <pthread.h>

void pthread_exit(void* retval);

retval和线程过程函数的返回值语义相同
```

注意：在任何线程中调用exit函数都将终止整个进程

### 线程执行轨迹

* 同步方式（非分离状态）：创建线程之后调用`pthread_join`函数等待其终止，并释放线程资源。
* 异步方式（分离状态）：无需创建者等待，线程终止后自行释放资源。

```
#include <pthread.h>

int pthread_detach(pthread_t thread);

返回值：成功返回0，失败返回错误码
```

使thread参数所标识的线程进入分离（DETACH）状态。处于分离状态的线程终止后自动释放线程资源，且不能被`pthread_join`函数等待。

### 取消线程

#### 向指定线程发送取消请求

```
#include <pthread.h>

int pthread_cancel(pthread_t thread);

返回值：成功返回0，失败返回错误码
```

注意：该函数只是向线程发出取消请求，并不等待线程终止。

缺省情况下，线程在收到取消请求后，并不会立即终止，而是仍继续运行，直到其达到某个取消点，在取消处线程检查其自身是否已经被取消了，并做出相应动作。当函数调用一些特定函数时，取消点会出现。

#### 设置调用线程的可取消状态

```
#include <pthread.h>

int pthread_setcancelstate(int state, int* oldstate);

返回值：成功返回0，并通过oldstate参数输出原可取消状态（若非NULL），失败返回错误码

state取值：
    PTHREAD_CANCAL_ENABLE  - 接受取消请求（缺省）
    PTHREAD_CANCEL_DISABLE - 忽略取消请求	
```

#### 设置调用线程的可取消类型

```
#include <pthread.h>

int pthread_setcanceltype(int type, int* oldtype);

返回值：成功返回0，并通过oldtype参数输出原可取消类型（若非NULL），失败返回错误码

type取值：
PTHREAD_CANCEL_DEFERRED 
		- 延迟取消（缺省），被取消线程在接收到取消请求后并不立即响应，而是一直等到执行了特定的函数（取消点）之后再响应该请求。
		
PTHREAD_CANCEL_ASYNCHRONOUS 
		- 异步取消，被取消线程可以在任意时间取消，不是非得遇到取消点才能被取消。但是操作系统并不能保证这一点。
```

### 线程属性

```
#include <pthread.h>

int pthread_create(
    pthread_t* restrict thread, 
    const phtread_t* restrict attr, 
    void* (*start_routine) (void*), 
    void* restrict arg
);
第二个参数即为线程属性，传NULL表示使用缺省属性

typedef struct {
	int                detachstate;  // 分离状态
	int                scope;        // 竞争范围
	int                inheritsched; // 继承特性
	int                schedpolicy;  // 调度策略
	struct sched_param schedparam;   // 调度参数
	size_t             guardsize;    // 栈尾警戒区大小（字节），缺省为一页（4096字节）
	void*              stackaddr;    // 栈地址
	size_t             stacksize;    // 栈大小
} pthread_attr_t;

分离状态
PTHREAD_CREATE_DETACHED - 分离线程
PTHREAD_CREATE_JOINABLE - 可汇合线程（缺省）

竞争范围
PTHREAD_SCOPE_SYSTEM               - 在系统范围内竞争资源
PTHREAD_SCOPE_PROCESS（Linux不支持） - 在进程范围内竞争资源

继承特性
PTHREAD_INHERIT_SCHED  - 调度属性从创建者线程继承（缺省）
PTHREAD_EXPLICIT_SCHED - 调度属性由后面两个成员确定

调度策略
SCHED_FIFO  - 先进先出策略。没有时间片。一个FIFO线程会持续运行，直到阻塞或有高优先级线程就绪。当FIFO线程阻塞时，系统将其移出就绪队列，待其恢复时再加到同优先级就绪队列末尾。当FIFO线程被高优先级线程抢占时，它在就绪队列中的位置不变。因此一旦高优先级线程终止或阻塞，被抢占的FIFO线程将会立即继续运行。
SCHED_RR    - 轮转策略。给每个RR线程分配一个时间片，一旦RR线程的时间片耗尽，系统即将移到就绪队列末尾。
SCHED_OTHER - 普通策略（缺省）。静态优先级为80，任何就绪的FIFO线程或RR线程都会抢占此类线程。

struct sched_param{
	int sched_priority; // 静态优先级，值越小优先级越高
};
```

不要手工读写该结构体，而应调用`pthread_attr_set/get`函数设置获取具体属性项。

#### 设置线程属性

* 初始话线程属性结构体：`int pthread_attr_init(pthread_attr_t* attr)`

* 设置具体线程属性项：

  ```
  int pthread_attr_setdetachstate (pthread_attr_t* attr, int detachstate)
  int pthread_attr_setscope       (pthread_attr_t* attr, int scope);
  int pthread_attr_setinheritsched(pthread_attr_t* attr, int inheritsched);
  int pthread_attr_setschedpolicy (pthread_attr_t* attr, int policy);
  int pthread_attr_setschedparam  (pthread_attr_t* attr, const struct sched_param* param);
  int pthread_attr_setstack       (pthread_attr_t* attr, void* stackaddr, size_t stacksize)
  ```

* 以设置好的线程属性结构体为参数创建线程

* 销毁线程属性结构体：`int pthread_attr_destroy(pthread_attr_t* attr)`

#### 获取线程属性

* 获取线程属性结构体：`int pthread_getattr_np(pthread_t thread, pthread_attr_t* attr)`
* 获取具体线程属性项

```
int pthread_attr_getdetachstate (pthread_attr_t* attr, int* detachstate);
int pthread_attr_getscope       (pthread_attr_t* attr, int* scope);
int pthread_attr_getinheritsched(pthread_attr_t* attr, int* inheritsched);
int pthread_attr_getschedpolicy (pthread_attr_t* attr, int* policy);
int pthread_attr_getschedparam  (pthread_attr_t* attr, struct sched_param* schedparam);
int pthread_attr_getstack       (pthread_attr_t* attr, void** stackaddr, size_t* stacksize);

以上所有函数成功返回0，失败返回错误码。
```

