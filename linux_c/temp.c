#include <stdio.h>
#include <stdlib.h>

typedef enum
{
	KEY1 = (1 << 1),
	KEY2 = (1<<11),
	KEY_ALL = KEY1 | KEY2
}Key;


int main()
{
    int a = -1;
    int b = ~(-1);
    int c = 1;
    int d = ~c;

    printf("a=%d\tb=%d\tc=%d\td=%d\t\n", a,b,c,d);

    printf("KEY1=%d\tKEY2=%d\tKEY_ALL=%d\t\n",KEY1,KEY2,KEY_ALL);

    exit(0);
}