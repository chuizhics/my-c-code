#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>


/**
 * 当键盘按下ctrl+c时，这个事件会被转化为一个信号，然后由操作系统内核来发送给相应的进程。
 * 操作系统内核负责管理所有的进程和资源，因此它是主导整个过程的实体。

    在Linux系统中，键盘驱动程序将ctrl+c事件转换为SIGINT信号并将其发送给前台进程组中的所有进程。
    前台进程组是当前具有控制终端的进程组，通常情况下，前台进程组是由终端启动的shell进程组。
    在终端中运行的所有命令和程序都会成为该shell进程组的成员。

    一旦信号被发送到进程，操作系统内核会在进程的用户栈上构建一个信号帧，其中包含了信号的详细信息，例如信号值、发送信号的进程ID等。
    然后，操作系统内核会为该进程设置一个相应的信号位，表示该进程已经接收到该信号。
    在进程下一次执行任何系统调用之前，内核会检查是否有未处理的信号，如果有，则会触发信号处理过程。

    进程可以通过注册信号处理函数或使用sigwait函数等待信号到达来检查是否有未处理的信号。
    当有信号到达时，进程的信号处理函数将被调用来处理信号，或者进程将从sigwait函数中返回，并执行相应的操作。
    进程可以选择忽略某些信号，例如通过调用signal函数并传递SIG_IGN参数来忽略信号。

    总之，当键盘按下ctrl+c时，整个信号传递和处理的过程是由操作系统内核主导的。
    操作系统内核会将ctrl+c事件转换为信号并将其发送给前台进程组中的所有进程，然后在进程的用户栈上构建信号帧，
    并为该进程设置一个相应的信号位，最终触发信号处理过程。
*/


static void int_handler(int s)
{
    write(STDOUT_FILENO,"!",1);
}   


int main()
{
    int i = 0;

    //signal(SIGINT,SIG_IGN);
    signal(SIGINT,int_handler);

    for(i = 0; i < 10; i++)
    {
        write(STDOUT_FILENO,"*",1);
        sleep(1);
    }


    exit(0);
}