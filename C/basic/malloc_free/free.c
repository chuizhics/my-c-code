#include <stdio.h>
#include <stdlib.h>

void func(int *p, int n)
{
    p = malloc(n);
    if(p == NULL)
        exit(1);
}


int main()
{
    int num = 40;
    int *p = NULL;
    func(p, num);
    free(p);

    exit(0);
}

#if 0
上述代码有两个问题：一是func函数没有正确地将动态分配的内存块的地址赋值给指针p；二是在main函数中释放了一个空指针。

为了解决这些问题，我们可以通过将指针p的地址传递给func函数，并在func函数中使用指针的地址来修改指针本身的值，从而让p指向动态分配的内存块。

以下是修改后的代码：
#include <stdio.h>
#include <stdlib.h>

void func(int **pp, int n)
{
    *pp = malloc(n * sizeof(int));
    if(*pp == NULL)
        exit(1);
}


int main()
{
    int num = 40;
    int *p = NULL;
    func(&p, num);
    // Do something with p
    free(p);

    exit(0);
}
在func函数中，我们将指向指针p的指针pp作为参数，并使用*pp来访问指针p所指向的内存地址，从而可以修改指针本身的值。
同时，我们将malloc函数的参数设置为n * sizeof(int)，以便正确地分配n个int类型的内存块。

在main函数中，我们将指针p的地址传递给func函数，并在func函数中将p指向动态分配的内存块。
注意，我们在使用指针p之前需要先检查它是否为NULL，以避免访问未分配的内存。最后，我们使用free函数释放指针p指向的内存块。
#endif