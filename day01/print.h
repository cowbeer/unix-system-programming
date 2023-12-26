#ifndef PRINT_H
#define PRINT_H
#include <stdio.h>
void print() {
    printf("__BASE_FILE__ : %s\n", __BASE_FILE__);
    printf("__FILE__ : %s\n", __FILE__);
    printf("__LINE__ : %d\n", __LINE__);
    printf("__FUNCTION__ : %s\n", __FUNCTION__);
    printf("__func__ : %s\n", __func__);
    printf("__DATE__ : %s\n", __DATE__);
    printf("__TIME__ : %s\n", __TIME__);
    printf("__INCLUDE_LEVEL__: %d\n", __INCLUDE_LEVEL__);
#ifdef __cplusplus
    printf("__cplusplus : %ld\n", __cplusplus);
#endif
}
#endif // PRINT_H
