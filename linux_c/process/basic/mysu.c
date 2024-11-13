#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * 除了常规的文件权限外，文件权限中还有一些特殊的权限位，例如：

    setuid权限位（s）：表示在执行该文件时，将以文件所有者的身份执行，而不是执行该文件的用户身份；
    setgid权限位（s）：表示在执行该文件时，将以该文件所在组的身份执行，而不是执行该文件的用户身份；
    sticky位权限位（t）：用于控制目录中文件的删除，只有文件所有者和超级用户才能删除该目录中的文件。
 * 
 * 
 * 
*/
/**
 * su root  //切换为root用户
 * 
 * chown root mysu  //更改文件所有者
 * 
 * chmod u+s mysu  //增加u+s权限
 * 
 * 
 *  root@ubuntu:/home/reyank/linux_c/process/basic# chown root mysu
    root@ubuntu:/home/reyank/linux_c/process/basic# ls -l mysu
    -rwxrwxr-x 1 root reyank 17040 4月   3 14:57 mysu
    root@ubuntu:/home/reyank/linux_c/process/basic# chmod u+s mysu
    root@ubuntu:/home/reyank/linux_c/process/basic# ls -l mysu
    -rwsrwxr-x 1 root reyank 17040 4月   3 14:57 mysu

 * 
*/
int main(int argc, char *argv[])
{
    pid_t pid;

    if (argc < 3)
    {
        // ./mysu john cat /tmp/out
        fprintf(stderr, "Usage:%s <uid> <command....>\n", argv[0]);
        exit(1);
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    else if (pid == 0)
    {
        if(setuid(atoi(argv[1])) != 0)
        {
            perror("setuid()");
            exit(1);
        }

        execvp(argv[2], argv + 2);
        perror("execvp()");
        exit(1);
    }
    else
    {
        wait(NULL);
    }

    exit(0);
}