#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    BIT_SET = 1,
    BIT_RESET = 0
} BitAction;


char* intToBinary(int num) {
    // 求出整数num在计算机中占据的二进制位数
    int bits = sizeof(int) * 8;

    // 为二进制字符串分配内存空间
    char* bin = (char*)malloc(bits + 1);
    if (!bin) {
        printf("Error: Memory allocation failed!");
        exit(1);
    }

    // 将整数num转换为二进制字符串
    for (int i = bits - 1; i >= 0; i--) {
        bin[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    // 添加字符串结尾标志
    bin[bits] = '\0';

    return bin;
}


int main() 
{
    int test = 2;
    BitAction r = (BitAction)test;

    printf("test = %d\n", (BitAction)test);
    printf("r = %d\n", r);

    int num = 42;
    char* bin = intToBinary(num);
    printf("%d in binary is: %s\n", num, bin);
    free(bin); // 释放内存空间
    return 0;
}