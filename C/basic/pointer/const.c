#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i = 10;

     printf("i = %d\n",i);

    int * const p = &i;  //指针常量 p不能变，*p可以变

    const int *const q = &i; //什么都不能变

    *p = 100;

//     int i = 1, j =100;
//     const int *p = &i; //常量指针  *p不能变，p可以变

//    F  *p = 10;
//     p = &j; 


    printf("i = %d  *p = %d\n",i, *p);


    // const float pi = 3.14;
    
    // float *p = &pi;
    // *p = 3.14159;

    // printf("%f\n", pi);

    exit(0);
}