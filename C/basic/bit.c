#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    int a = 4, b = 3;
    int num = a | b;
    char* bina = intToBinary(a);
    printf("%d in binary is: %s\n", a, bina);

    char* binb = intToBinary(b);
    printf("%d in binary is: %s\n", b, binb);

    char* bin = intToBinary(num);
    printf("%d in binary is: %s\n", num, bin);

    free(bina);
    free(binb);
    free(bin); // 释放内存空间
    return 0;
}