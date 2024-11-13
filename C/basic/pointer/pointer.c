
#include <stdio.h>
#include <stdlib.h>


int add(int m, int n)
{
    return m +n;
}

int sub(int m, int n)
{
    return m - n;
}


void dofunp(int (*p[])(int, int), size_t size, int m, int n)
{
    int i;
    for ( i = 0; i < size; i++)
    {
        printf("p[%d](m,n) = %d \n", i,p[i](m,n));
    }
    
}

#if 0
int i = 15;


static void fun1()
{
    printf("[%s:%d]i = %d\n",__FUNCTION__,__LINE__,i);
    int i = 4;
    printf("[%s:%d]i = %d\n",__FUNCTION__,__LINE__,i);
}


static void fun2()
{
    printf("[%s:%d]i = %d\n",__FUNCTION__,__LINE__,i);
    i -= 10;
}
#endif


int main()
{

        int m =5, n = 7;

        int (*padd)(int, int) = add;
        int (*psub)(int, int) = sub;

        int (*parr[])(int, int) = {padd, psub};


        printf("add(m,n) : m + n = %d\n", add(m,n));
        printf("padd(m,n) : m + n = %d\n", padd(m,n));

        printf("sub(m,n) : m - n = %d\n", sub(m,n));
        printf("psub(m-n) : m - n = %d\n", psub(m,n));

        dofunp(parr,sizeof(parr)/sizeof(*parr),m,n);





//     fun1();
//     fun2();
//     fun1();
//     fun2();


    // int s = 0;
    // int i = 1;
    // int j = 5;
    // int k = 7;

    // int * arr[3] = {&i, &j, &k};

    // for(s = 0; s < 3; s++)
    // {
    //     printf("%p-->%d\t",arr[s], *arr[s]);
    // }
    // printf("\n");

    // int a[6] = {5, 1, 7, 2, 8, 3};
    // int *p = &a[1];
    // int y;
    // y = (*--p)++;

    // printf("y = %d\n", y);
    // //int *q = &a;
    // printf("%p-->%p\n", a, &a);
    // printf("a[0] = %d\n", a[0]);

    // printf("y = %d\n", y);

#if 0
    int i = 1;
    int *p = &i;
    float *f = NULL;
    double *d = NULL;
    char *c = NULL;

    int **q = &p;

    printf("i = %d\n", i);
    printf("*p = %d\n", *p);
    printf("&p = %p\n", &p);
    printf("&i = %p\n", &i);
    printf("p = %p\n", p);
#endif
    exit(0);
}