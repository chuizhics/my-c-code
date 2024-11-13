#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <syslog.h>

#include <time.h>
#include <string.h>
#include <errno.h>

#include <signal.h>

#define FNAME "/tmp/out"
#define WORKINGDIRECTORY "/"

static FILE *fp = NULL;
static timer_t timerid;
static int timerid_inited  = 0;

static volatile sig_atomic_t i = 0;

static int daemonize(const char *dir)
{
    pid_t pid;
    int fd;

    pid = fork();
    if (pid < 0)
    {
        return -1;
    }
    else if (pid == 0)
    {
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        fd = open("/dev/null", O_RDWR);
        if (fd < 0)
        {
            return -1;
        }
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            return -1;
        }
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            return -1;
        }
        if (dup2(fd, STDERR_FILENO) == -1)
        {
            return -1;
        }

        if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
        {
            close(fd);
        }

        if (chdir(dir) == -1)
        {
            return -1;
        }
        if (setsid() < 0)
        {
            return -1;
        }
        return 0;
    }
    else
    {
        // return 0;
        exit(0); //  应该是调用进程退出 子进程运行
    }
}

static void daemon_exit(int sig)
{
    syslog(LOG_INFO, "<info> exiting....");
    if (fp != NULL)
    {   
        syslog(LOG_INFO, "<info> close fp");
        fclose(fp);
    }
    
    if(timerid_inited)
    {
        syslog(LOG_INFO,"<info> close timer.");
        timer_delete(timerid);
    }
    syslog(LOG_INFO, "<info> exited....");
    closelog();
    exit(0);
}

static void register_signal(void)
{
    struct sigaction sact;

    // nsact.sa_flags = SA_SIGINFO;
    sact.sa_flags = 0;
    sact.sa_handler = daemon_exit;
    
    sigemptyset(&sact.sa_mask);
    sigaddset(&sact.sa_mask, SIGINT);
    sigaddset(&sact.sa_mask, SIGQUIT);
    sigaddset(&sact.sa_mask, SIGTERM);

    sigaction(SIGINT, &sact, NULL);
    sigaction(SIGQUIT, &sact, NULL);
    sigaction(SIGTERM, &sact, NULL);
}

static void do_real_task(int sig,siginfo_t *si, void *uc)
{
    // syslog(LOG_INFO,"<info> do_real_task()...");
    fprintf(fp,"%d\n",i++);
    fflush(fp);
}


static void register_timer_task(void)
{
    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec its;
   
    int sec = 1;
    int signal_num;

    // 设置定时器事件
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;

    // 创建定时器
    timer_create(CLOCK_REALTIME, &sev, &timerid);
    timerid_inited = 1;

    // 设置定时器
    its.it_value.tv_sec = sec; // 初次到期时间
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = sec; // 重复间隔时间
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);

    // 注册信号处理函数

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = do_real_task;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) == -1) 
    {
        //perror("sigaction");
        syslog(LOG_ERR,"sigaction():%s",strerror(errno));
        daemon_exit(0);
    }
    // 等待定时器
    while (1) {
        syslog(LOG_INFO,"<info> wait signal....");
        sigwait(&sa.sa_mask,&signal_num);
        // F sigwait(NULL,NULL);
        // pause();
        syslog(LOG_INFO,"<info> received signal: %d\n", signal_num);
    }
}

int main()
{
    openlog("mydaemon", LOG_PID, LOG_DAEMON);

    // 只输出 LOG_INFO 及以下级别的日志
    setlogmask(LOG_UPTO(LOG_INFO));

    register_signal();

    if (daemonize(WORKINGDIRECTORY) < 0)
    {
        syslog(LOG_ERR, "<error> daemonize failed.");
        //exit(1);
        daemon_exit(1);
    }

    syslog(LOG_INFO, "<info> daemonize() success.");

    fp = fopen(FNAME, "w");
    if (fp == NULL)
    {
        syslog(LOG_ERR, "<error> fopen():%s", strerror(errno));
        daemon_exit(1);
    }

    syslog(LOG_INFO, "<info> %s was opened.", FNAME);

    register_timer_task();


    exit(0);
}