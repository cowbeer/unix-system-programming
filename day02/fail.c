#include <stdio.h>

int intmod(int a, int b, int* mod){
    if (!b) {
        return -1;
    }
    *mod = a % b;
    return 0;
}


int main(void) {
    printf("input two integers:");
    int a,b;
    scanf("%d%d", &a, &b);
    int mod;
    int ret = intmod(a, b, &mod);
    if (ret < 0) {
        printf("failed to mod\n");
	return -1;
    }
    printf("%d mod %d = %d\n", a, b, mod);
    return 0;
}
