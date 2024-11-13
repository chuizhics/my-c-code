
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

#define MYRTSIG     (SIGRTMIN+6)

static void myrtsig_handler(int s)
{
    write(STDOUT_FILENO, "!", 1);
}
/**
 * int sigsuspend(const sigset_t *mask);
 * 
 * sigsuspend() 临时用 mask 给出的掩码替换调用线程的信号掩码，然后挂起线程直到传递一个信号，该信号的作用是调用信号处理程序或终止进程。

    如果信号终止进程，则 sigsuspend() 不会返回。如果捕获到信号，
    则 sigsuspend() 在信号处理程序返回后返回，并且信号掩码恢复到调用 sigsuspend() 之前的状态。

    无法阻止 SIGKILL 或 SIGSTOP；在掩码中指定这些信号对线程的信号掩码没有影响。
 * 
 * 
*/
int main()
{
    int i = 0, j = 0;

    sigset_t set, oset, saveset;

    signal(MYRTSIG, myrtsig_handler);

    sigemptyset(&set);

    sigaddset(&set, MYRTSIG);

    sigprocmask(SIG_UNBLOCK, &set, &saveset);  // 保存旧值

    sigprocmask(SIG_BLOCK, &set, &oset);
    for (i = 0; i < 1000; i++)
    {
        
        for (j = 0; j < 5; j++)
        {
            write(STDOUT_FILENO, "*", 1);
            sleep(1);
        }
        write(STDOUT_FILENO, "\n", 1);
        sigsuspend(&oset);
    }

    sigprocmask(SIG_SETMASK, &saveset, NULL);  //恢复现场

    exit(0);
}