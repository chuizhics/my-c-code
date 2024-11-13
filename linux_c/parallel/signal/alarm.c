#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{


    // alarm + pause 可实现sleep
    alarm(10);
    alarm(1);
    alarm(5);  // finally

    while (1)
    {
        pause();
    }

    exit(0);
}