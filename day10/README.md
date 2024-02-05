# 线程同步

## 竞争与同步

当多个线程同时访问其所共享的进程资源时，需要互相协调，以防止出现数据不一致、不完整的问题。这就叫线程同步。

## 信号量

信号量是一个计数器，用于控制访问有限共享资源的线程数。

```
#include <semaphore.h>

// 创建信号量
int sem_init(sem_t* sem, int pshared, unsigned int value);

sem：信号量ID，输出
pshared：一般取0，表示调用线程的信号量。非0表示该信号量可以共享内存的方式，为多个进程所共享（Linux暂不支持）
value：信号量初值

// 信号量减1，不够减就阻塞
int sem_wait(sem_t* sem);

// 信号量减1，不够减即返回-1，errno为EAGAIN
int sem_trywait(sem_t* sem);

// 信号量减1，不够减就阻塞
// 直到abs_timeout超时时返回-1，errno为ETIMEDOUT
int sem_timedwait(sem_t* sem, const struct timespec* abd_timeout);

struct timespec {
	time_t tv_sec; // seconds
	long tv_nsec; // Nanoseconds[0-999999999]
};

// 信号量加1
int sem_post(sem_t* sem);
```

注意：

信号量APIs没有声明在pthread.h中，而是声明在semaphore.h中，失败也不返回错误码，而是返回-1，同时设置errno。

互斥量任何时候都只允许一个线程访问共享资源，而信号量则允许最多value个线程同时访问共享资源，value为1时，与互斥量等价。

## 条件变量

### 生产者消费者模型

生产者：产生数据的线程。

消费者：使用数据的线程。

通过缓冲区隔离生产者和消费者，与二者直连相比，避免相互等待，提高运行效率。

生产快于消费，缓冲区满，撑死。

消费快于生产，缓冲区空，饿死。

条件变量可以让调用线程在满足特定条件的情况下暂停。

```
#include <pthread.h>

int pthread_cond_init(pthread_cont_t* cond, const pthread_conattr_t* attr);

pthread_cont_t cond = PTHREAD_COND_INITIALIZER;

// 使调用线程睡入条件变量cond，同时释放互斥锁mutex
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime);
int pthread_cont_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime);

struct timespec {
	time_t tv_sec; // seconds
	long tv_nsec; // nanoseconds [0-999999999]
};

// 从条件变量cond中唤出一个线程，令其重新获得原先的互斥锁
int pthread_cond_signal(pthread_cond_t* cond);
注意：被唤出的线程此刻将从pthread_cond_wait函数中返回，但如果该线程无法获得原先的锁，则会继续阻塞在加锁上。

// 从条件变量cond中唤出所有线程
int pthread_cond_broadcast(pthread_cond_t* cond);
int pthread_cond_destroy(pthread_cond_t* cond);
注意：当一个线程被从条件变量中唤出后，导致其睡入条件变量的条件可能还需要再判断一次，因其随时有可能被其他线程修改。
```

### 哲学家就餐问题

