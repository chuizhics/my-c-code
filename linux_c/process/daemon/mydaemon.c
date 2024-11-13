#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <syslog.h>

#include <string.h>
#include <errno.h>


#define FNAME "/tmp/out"
#define WORKINGDIRECTORY "/"


static int daemonize(const char *dir)
{
    pid_t pid;
    int fd;

    pid = fork();
    if (pid < 0)
    {
        //perror("fork()");
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
            // perror("open()");
            return -1;
        }
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            // perror("dup2");
            return -1;
        }
        if (dup2(fd,  STDOUT_FILENO) == -1)
        {
            // perror("dup2");
            return -1;
        }
        if (dup2(fd, STDERR_FILENO) == -1)
        {
            // perror("dup2");
            return -1;
        }

        if(fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
        {
            close(fd);
        }

        if (chdir(dir) == -1) 
        {
            // perror("chdir");
            return -1;
        }
        if (setsid() < 0) 
        {
            // perror("setsid()");
            return -1;
        }
    //    umask(0); 限制权限
        return 0;
    }
    else
    {
        // return 0;  
        /**!!!!!!! **/
        exit(0);  //  应该是调用进程退出 子进程运行
    }
}

int main()
{
    FILE *fp = NULL;
    int i = 0;

    openlog("mydaemon",LOG_PID,LOG_DAEMON);


    // 只输出 LOG_INFO 及以下级别的日志
    setlogmask(LOG_UPTO(LOG_INFO));

    if (daemonize(WORKINGDIRECTORY) < 0)
    {
        syslog(LOG_ERR,"<error> daemonize failed.");
        // fprintf(stderr, "daemonize failed.\n");
        exit(1);
    }
    else
    {
        syslog(LOG_INFO,"<info> daemonize() success.");
    }

    fp = fopen(FNAME, "w");
    if (fp == NULL)
    {
        syslog(LOG_ERR,"<error> fopen():%s",strerror(errno));
        // perror("fopen()");
        exit(1);
    }

    syslog(LOG_INFO,"<info> %s was opened.",FNAME);

    while (1)
    {
        fprintf(fp, "%d\n", i++);
        fflush(fp);
        // syslog(LOG_DEBUG,"<debug> %d is printed.", i);
        sleep(1);
    }
    fclose(fp);
    closelog();
    exit(0);
}