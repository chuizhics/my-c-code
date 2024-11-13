#include <stdio.h>
#include <stdlib.h>

/**
 * 
 * system: fork + exec + wait 的封装
 * 
 * The  system()  library  function uses fork(2) to create a child process
       that executes the shell command specified in command using execl(3)  as
       follows:

           execl("/bin/sh", "sh", "-c", command, (char *) NULL);

       system() returns after the command has been completed.

*/

int main()
{

    system("date +%s > /tmp/out");

    exit(0);
}