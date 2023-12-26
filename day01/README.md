# Day01

## 头文件

头文件的作用？

* 声明外部变量、函数和类
* 定义宏、类型别名和自定义类型
* 包含其他头文件
* 借助头文件卫士，防止因同一个文件被多次包含而引发重定义错误

包含头文件注意：

* `gcc -I` 
  * 指定头文件附加搜索路径
* `#include <...>`
  * 先找`-I`指定的目录，再找系统目录
* `#include "..."`
  * 先找`-I`指定的目录，再找当前目录，最后找系统目录

头文件的三种定位方式：

* `#include "目录/xxx.h"` 头文件路径发生变化，需要修改源程序
* `C_INCLUDE_PATH/CPATH=目录` 同时构建多个工程可能引发冲突
* `gcc -I目录` 既不用修改程序，也不会有冲突

头文件的系统目录

```
/usr/include
/usr/local/include
/usr/lib/gcc/i686-linux-gnu/4.6.3/include
/usr/include/c++/4.6.3
```

```
#pragma GCC dependency <文件>   若文件比当前文件新，则产生警告
#pragma GCC poison <标识>       若出现标识，则产生错误
#pragma pack(1/2/4/8)          按1/2/4/8字节对齐补齐
```

预定义宏

```
__BASE_FILE__       正在编译的文件名
__FILE__            所在文件名
__LINE__            行号
__FUNCTION__        函数名
__func__            同__FUNCTION__
__DATE__            编译日期
__TIME__            编译时间
__INCLUDE_LEVEL__   包含层数    
__cplusplus         C++编译中定义，为正数

C_INCLUDE_PATH      C头文件的附加搜索路径，相当于gcc -I选项
CPATH               同C_INCLUDE_PATH
CPLUS_INCLUDE_PATH  C++头文件附加搜索路径

LIBRARY_PATH        链接时查找静态库或动态库的路径
LD_LIBRARY_PATH     运行时查找共享库的路径
```

## 库

链接静态库是将库中的被调用代码复制到调用模块中，而链接共享库则只是在调用模块中，嵌入被调用代码在库中的（相对）地址。静态库占用空间非常大，不易修改，但执行效率高。共享库占用空间小，易于修改，但执行效率低。静态库的缺省扩展名是`.a`，共享库的缺省扩展名是`.so`。

### 静态库

* 编写源程序：`.c` `.h`
* 编译成目标文件：`gcc -c xxx.c` -> `xxx.o`
* 打包成静态文件： `ar -r libxxx.a xxx.0 ...`

ar指令：ar [选项] 静态库文件名 目标文件列表

`-r`：将目标文件插入到静态库中，已存在则更新

`-q`：将目标文件追加到静态库尾

`-d`：从静态库中删除目标文件

`-t`：列表显示静态库中的目标文件

`-x`：将静态库展开为目标文件

注意：提供静态库的同时也需要提供头文件。

```
以下三种方式都能编译
// 1参数法
gcc main.c -lmath -L.
// 2直接法
gcc main.c libmath.a
// 3环境法
export LIBRARY_PATH=$LIBRARY_PATH:.
gcc main.c -lmath
```

### 动态库

* 编写源程序：`.c` `.h`
* 编译成目标文件：`gcc -c -fpic xxx.c` -> `xxx.o`

* 链接成共享库文件： `gcc -shared xxx.o ... -o libxxx.so`

或一次完成：`gcc -shared -fpic calc.c show.c -o libmath.so`

PIC（Position Independent Code）：位置无关代码。可执行程序加载它们时，可将其映射到其他地址空间的任何位置。

* `-fPIC`：大模式，生成代码比较大，运行速度比较慢，所有平台都支持。
* `-fpic`：小模式，生成代码比较小，运行速度比较快，仅部分平台支持。

```
gcc main.c -lmath -L.
运行时需要保证LD_LIBRARY_PATH环境变量中包含共享库所在的路径：
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
./a.out

在可执行程序的链接阶段，并不将所调用函数的二进制代码复制到可执行程序中，只是将该函数在共享库中的地址嵌入到可执行程序中，因此运行时需要依赖共享库。
```

gcc缺省链接共享库，可通过`-static`选项强制链接静态库

加载共享库

```
void* dlopen(const char* filename, int flag);
filename：
	共享库路径，若只给文件名，则根据LD_LIBRARY_PATH环境变量搜索
flag：
	加载方式：
		RTLD_LAZY - 延迟加载，使用共享库中的符号（如函数调用）时才加载
		RTLD_NOW - 立即加载
返回值：
	成功返回库的句柄，失败返回NULL
```

```
void* dlsym(void* handle, const char* symbol);
handle：共享库句柄
symbol：函数名
成功返回函数地址，失败返回NULL
```

```
int dlclose(void* handle);
handle：共享库句柄
成功返回0，失败返回非零
```

```
char* dlerror(void);
有错误发生则返回字符信息字符串指针，否则返回NULL
```

注意：链接时不再需要`-lmath`，但需要`-ldl`。
