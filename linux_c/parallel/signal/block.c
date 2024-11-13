
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

static void int_handler(int s)
{
    write(STDOUT_FILENO, "!", 1);
}

int main()
{
    int i = 0, j = 0;

    sigset_t set, oset, saveset;

    signal(SIGINT, int_handler);

    sigemptyset(&set);

    sigaddset(&set, SIGINT);

    sigprocmask(SIG_UNBLOCK, &set, &saveset);  // 保存旧值

    for (i = 0; i < 1000; i++)
    {
        sigprocmask(SIG_BLOCK, &set, &oset);
        for (j = 0; j < 5; j++)
        {
            write(STDOUT_FILENO, "*", 1);
            sleep(1);
        }
        write(STDOUT_FILENO, "\n", 1);
        // sigprocmask(SIG_UNBLOCK,&set,NULL);
        sigprocmask(SIG_SETMASK, &oset, NULL);
    }

    sigprocmask(SIG_SETMASK, &saveset, NULL);  //恢复现场

    exit(0);
}