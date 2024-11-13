#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i = 3;

    printf("i = %d\n", i);
    printf("你好\n");
    exit(0);
}



#if 0
void func(void)
{
    static int x = 0;
    x = x+1;
    printf("%p->%d\n", &x, x);
}

int main()
{
    func();
    func();
    func();

    exit(0);
}

#endif