#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    putchar('a');
    write(1,"b",1);   // 0--stdin 1--stdout 2--stderr
    
    putchar('a');
    write(1,"b",1);

    putchar('a');
    write(1,"b",1);
    
    putchar('\n');
    exit(EXIT_SUCCESS);
}