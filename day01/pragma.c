#include <stdio.h>

// 若dep.c比当前文件新则产生警告
#pragma GCC dependency "dep.c"

// 若出现goto/float则报错
#pragma GCC poison goto float

int main(void) {
//    float f = 3.14;
//    goto escape;
    printf("sizeof(double)=%ld, sizeof(char)=%lu, sizeof(int)=%lu, sizeof(short)=%lu\n", sizeof(double), sizeof(char), sizeof(int), sizeof(short));

#pragma pack(1)
    struct S1 {
        double d;
	char c;
	int i;
	short h;
    };
    printf("%lu\n", sizeof(struct S1));

    // 按默认对齐
#pragma pack()
    struct S2 {
        double d;
	char c;
	int i;
	short h;
    };
    printf("%lu\n", sizeof(struct S2));
    return 0;
escape:
    printf("%d\n", __LINE__);
    return 0;
}
