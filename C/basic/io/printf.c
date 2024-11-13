#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    int i = 255;
    char ch = 65;
    float f = 123.456;
    double d = 12.4520;

    //printf("11111");  要加\n  强制刷新缓冲区
    printf("[%s:%d] before while().\n", __FUNCTION__,__LINE__);
    // while (1)
    // {
    //     /* code */
    // }

    // sleep(5);


    printf("[%s:%d] after while().\n", __FUNCTION__,__LINE__);

    // printf("ch = %d\n", ch);


    // printf("ch = %c\n", ch);

    // printf("-128 = %c\n",-128);

    // printf("1 = %c\n",1);

    // printf("%d %e \n", i, f, d);



    exit(0);
}
