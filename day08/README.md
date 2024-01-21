# 网络通信

## 基本概念

### IOS/OSI七层网络协议模型

```
应用层     Application
表示层     Presentation     高层
会话层     Session
----------------------------------
传输层     Transport
网络层     Network          底层
数据链路层  Data Link
物理层     Physical
```

### TCP/IP协议族

TCP（Transmission Control Protocol，传输控制协议）面向连接的服务。能保证接收到的数据顺序与发送顺序一致。可靠，慢。

UDP（User Datagram Protocol，用户数据报文协议）面向无连接的服务。不能保证接收数据的顺序。不可靠，快。

IP（Internet Protocol，互联网协议）信息传输机制。

### TCP/IP协议与ISO/OSI模型的对比

```
ISO/OSI     TCP/IP
-----------------------------------------
应用层
表示层       应用层        TELNET/FTP/HTTP
会话层
-----------------------------------------
传输层       传输层        TCP/UDP
-----------------------------------------
网络层       互联网层       IP/路由
-----------------------------------------
数据链路层    网络接口层     驱动/设备
物理层
```

### 消息包

```
TELNET/FTP/HTTP
-----------------
TCP/UDP
-----------------
IP
-----------------
ETHERNET
```

从上至下，消息包逐层递增，从下至上，消息包逐层减少。

### IP地址

IP地址是Internet中唯一的地址标识

* 一个IP地址占32位，正在扩充至128位
* 每个Internet包必须带IP地址

电分十进制

0x01020304->1.2.3.4，高数位在左，低数位在右

IP地址分级

A级：0+7位网络地址+24位本机地址

B级：10+14位网络地址+16位本机地址

C级：110+21位网络地址+8位本机地址

D级：1110+28位多播（Multicast）地址

子网掩码

IP地址 & 子网掩码 = 网络地址

IP地址：192.168.182.48

子网掩码：255.255.255.0

网络地址：192.168.182

本地地址：48

### 套接字

#### 接口

```
PuTTY   -> telnet \
LeapFTP -> ftp     -> socket -> TCP/UDP -> IP -> 网卡驱动 -> 网卡硬件
IE      -> http   /
```

#### 异构

Java@Unix -> socket  <-----> socket <- C/C++ @Windows

#### 模式

点对点（Peer-to-Peer，P2P）：一对一的通信。

客户机/服务器（Client/Server，C/S）：一对多的通信。

#### 绑定

先要有一个套接字描述符，还要有物理通信载体，然后将二者绑定在一起。

#### 函数

##### 创建套接字

```
#include <sys/socket.h>

intsocket(int domain, int type, int protocol);

返回值：成功返回套接字描述符，失败返回-1

domain - 域地址族，取值：
	AF_UNIX/AF_LOCAL/AF_FILE：本地通信（进程间通信）
	AF_INET：基于TCP/IPv4（32位IP地址）的网络通信
	AF_INET6：基于TCP/IPv6（128位IP地址）的网络通信
	AF_PACKET：基于地泵包接口的网络通信

type - 通信协议，取值：
	SOCK_STREAM：数据流协议，即TCP协议
	SOCK_DGRAM：数据报协议，即UDP协议
	
protocol - 特别通信协议，一般不用，即0即可
```

套接字描述符类似于文件描述符，UNIX把网络当文件看待，发送数据即写文件，接收数据即读文件，一切皆文件。

##### 准备通信地址

基本地址类型

```
struct sockaddr {
	sa_family_t sa_family;   // 地址族
	char        sa_data[14]; // 地址值
};
```

本地地址类型

```
#include <sys/un.h>
struct sockaddr_un {
	sa_family_t sun_family; // 地址族
	char        sun_path[]; // 套接字文件路径
};
```

网络地址类型

```
#include <netinet/in.h>

struct sockaddr_in {
	sa_family_t     sin_family; // 地地址族
	in_port_t       sin_port;   // 端口号，逻辑上表示一个参与通信的进程
	struct in_addr  sin_addr;   // IP地址
};

typedef uint32_t in_addr_t;
struct in_addr {
	in_addr_t s_addr;
};

IP地址用于定位主机，端口号用于定位主机上的进程
```

##### 将套接字和通信地址绑定在一起

```
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

返回值：成功返回0，失败返回-1
```

##### 建立连接

```
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

返回值：成功返回0，失败返回-1
```

##### 用读写文件的方式通信：read/write

##### 关闭套接字：close

##### 字节序转换

```
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
```

主机字节序因处理器架构而异，有的采用小端字节序，有的采用大端字节序。网络字节序则固定采用大端字节序。

##### IP地址转换

```
#include <arpa/inet.h>

// 点分十进制字符串 -> 网络字节序32位无符号整数
in_addr_t inet_addr(const char* cp);

// 点分十进制字符串 -> 网络字节序32位无符号整数
int inet_aton(const char* cp, struct in_addr* inp);

// 网络字节序32位无符号整数 -> 点分十进制字符串
char* inet_ntoa(struct in_addr in);
```

##### 编程模型

本地通信

服务器：创建套接字（AF_LOCAL）-> 准备地址并绑定（sockaddr_un）-> 接收数据 -> 关闭套接字

客户机：创建套接字（AF_LOCAL）-> 准备地址并连接（sockaddr_un）-> 发送数据 -> 关闭套接字

网络通信

服务器：创建套接字（AF_INET）-> 准备地址并绑定（sockaddr_in）-> 接收数据 -> 关闭套接字

客户机：创建套接字（AF_INET）-> 准备地址并连接（sockaddr_in）-> 发送数据 -> 关闭套接字

## 基于TCP协议的客户机/服务器模型

### 基本特征

* 面向连接

* 可靠，保证数据的完整性和有序性。每个发送都有应答，若在时间窗口内没有收到应答，则重新发送。

### 编程模型

| 步骤 | 服务器     | 服务器函数 | 客户机函数 | 客户机     | 步骤 |
| ---- | ---------- | ---------- | ---------- | ---------- | ---- |
| 1    | 创建套接字 | socket     | socket     | 创建套接字 | 1    |
| 2    | 准备地址   | ...        | ...        | 准备地址   | 2    |
| 3    | 绑定套接字 | bind       | /          | /          | /    |
| 4    | 监听套接字 | listen     | /          | /          | /    |
| 5    | 接受连接   | accept     | connect    | 建立连接   | 3    |
| 6    | 接收请求   | recv       | send       | 发送请求   | 4    |
| 7    | 发送响应   | send       | recv       | 接收响应   | 5    |
| 8    | 关闭套接字 | close      | close      | 关闭套接字 | 6    |

### 常用函数

```
#include <sys/socket.h>

int listen(int sodkfd, int blocklog);

返回值：成功返回0，失败返回-1
```

* 将sockfd参数所标识的套接字标记为被动模式，使之可用于接受连接请求。
* blocklog参数表示未决连接请求队列的最大长度，即最多允许同时多少个未决连接请求存在。若服务器端的未决连接数已达到此限，则客户机端的connect函数返回-1且errno为ECONNREFUSED。

```
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);

返回值：成功返回通信套接字描述符，失败返回-1
```

* 从sodkfd参数所标识套接字的未决连接请求队列中，提取第一个连接请求，同时创建一个新的套接字，用于在该链接中通信，返回该套接字的描述符。

```
#include <sys/socket.h>

ssize_t recv(int sockfd, void* buf, size_t len, int flags);

返回值：成功返回实际接收到的字节数，失败返回-1
```

* 通过sockfd参数所标识的套接字，期望接收len个字节到buf所指向的缓冲区中。

```
#include <sys/socket.h>

ssize_t send(int sockfd, const void* buf, size_t len, int flags);

返回值：成功返回实际发送的字节数，失败返回-1
```

* 通过sockfd参数所标识的套接字，从buf所指向的缓冲区中发送len个字节。

## 基于UDP协议的客户机/服务器模型

### 基本特征

* 无连接
* 不可靠，不保证数据的完整性和有序性。效率高，速度快。

### 编程模型

| 步骤 | 服务器     | 服务器函数 | 客户机函数 | 客户机     | 步骤 |
| ---- | ---------- | ---------- | ---------- | ---------- | ---- |
| 1    | 创建套接字 | socket     | socket     | 创建套接字 | 1    |
| 2    | 准备地址   | ...        | ...        | 准备地址   | 2    |
| 3    | 绑定套接字 | bind       | /          | /          | /    |
| 4    | 接收请求   | recvfrom   | sendto     | 发送请求   | 3    |
| 5    | 发送响应   | sendto     | recvfrom   | 接收响应   | 4    |
| 6    | 关闭套接字 | close      | close      | 关闭套接字 | 5    |

### 常用函数

```
#include <sys/socket.h>

ssize_t recvfrom(
	int               sockfd, 
	void*             buf, 
	size_t            len, 
	int               flags, 
	struct sockaddr*  src_addr, 
	socketlen_t*      addrlen
);

返回值：成功返回实际接收到的字节数，失败返回-1
```

* 通过sockfd参数所标识的套接字，期望接收len个字节到buf所指向的缓冲区中

* 若src_addr和addrlen参数不是空指针，则通过这两个参数输出源地址接口及其长度。注意：在这种情况下，addrlen参数的目标应被初始化为src_addr参数的目标数据结构的大小。

```
#include <sys/socket.h>

ssize_t sendto(
	int                     sockfd,
	const void*             buf,
	size_t                  len,
	int                     flags,
	const struct sockaddr*  dest_addr,
	socklen_t               addrlen
);

返回值：成功返回实际发送的字节数，失败返回-1
```

* 通过sockfd参数所标识的套接字，从buf所指向的缓冲区中发送len个字节
* 发送目的地址结构及其长度通过dest_addr和addr_len参数输入