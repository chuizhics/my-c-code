#include <stdio.h>

int main() {
    unsigned long long num = 0x55842c3c16b0;
    printf("0x%016llx\n", num); // 输出带有前导0的16位十六进制数
    return 0;
}