#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <pthread.h>
#include <signal.h>

#include "relayer.h"

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"

#define TTY3 "/dev/tty10"
#define TTY4 "/dev/tty9"

#define BUFSIZE 1024

static int fd1 = -1;
static int fd2 = -1;
static int job1 = -1;

static int fd3 = -1;
static int fd4 = -1;
static int job2 = -1;

static int quitflag = 0; /*set nonzero by thread*/
static pthread_mutex_t mut_quitflag = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_quitflag = PTHREAD_COND_INITIALIZER;

static sigset_t mask_save;

static void diff(struct timespec start, struct timespec end, struct timespec *elapsed_time)
{
    if(elapsed_time != NULL)
    {
        if ((end.tv_nsec - start.tv_nsec) < 0) {
        elapsed_time->tv_sec = end.tv_sec - start.tv_sec - 1;
        elapsed_time->tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
        } else {
            elapsed_time->tv_sec = end.tv_sec - start.tv_sec;
            elapsed_time->tv_nsec = end.tv_nsec - start.tv_nsec;
        }
    }
}

static void print_jobstat(struct rel_stat_st *job_stat)
{
    struct timespec elapsed_time;
    double seconds;
    if(job_stat != NULL)
    {
        diff(job_stat->time_start, job_stat->time_end, &elapsed_time);
        seconds = elapsed_time.tv_sec + (double) elapsed_time.tv_nsec / 1000000000.0;
        printf("job_state:\n");
        printf("Real Time: %f seconds\n", seconds);
        printf("job state: %d\n",job_stat->state);
        printf("job_stat->count12 = %lld\n",job_stat->count12);
        printf("job_stat->count21 = %lld\n",job_stat->count21);
    }
}

static void main_safe_exit(void)
{
    int err;
    printf("[main_safe_exit:%d] enter main_safe_exit\n", __LINE__);
    if (job2 >= 0)
    {
        rel_waitjob(job2, NULL);
    }
    if (fd4 >= 0)
    {
        close(fd4);
    }
    if (fd3 >= 0)
    {
        close(fd3);
    }

    if (job1 >= 0)
    {
        rel_waitjob(job1, NULL);
    }
    if (fd2 >= 0)
    {
        close(fd2);
    }
    if (fd1 >= 0)
    {
        close(fd1);
    }
    err = pthread_sigmask(SIG_SETMASK, &mask_save, NULL);
    if (err)
    {
        fprintf(stderr, "main:pthread_sigmask():%s\n", strerror(err));
        fflush(stderr);
        _exit(1);
    }
    printf("[main_safe_exit:%d] exit main_safe_exit\n", __LINE__);
}


static void thr_safe_exit(void)
{
    // fprintf(stdout, "received signal %d\n", signo);
    // pthread_mutex_lock(&mut_num);

    // num = -1; // 向除主线程外的其余线程发送退出信号
    // pthread_cond_broadcast(&cond_num);

    // pthread_mutex_unlock(&mut_num);

    pthread_mutex_lock(&mut_quitflag);

    quitflag = 1; // 向主线程发送退出信号
    pthread_cond_broadcast(&cond_quitflag);

    pthread_mutex_unlock(&mut_quitflag);

}


static void menu(void)
{
    while(1)
    {
        printf("/****************menu******************/\n");



    }
}


static void *thr_signal_handler(void *arg)
{
    int i, err, signo;
    //syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] enter thr_signal_handler", __LINE__);
    printf("[thr_signal_handler:%d] enter thr_signal_handler\n", __LINE__);
    sigset_t signal_handler_mask;


    sigemptyset(&signal_handler_mask);
    sigaddset(&signal_handler_mask, SIGINT);
    sigaddset(&signal_handler_mask, SIGTERM);
    sigaddset(&signal_handler_mask, SIGQUIT);

    while (1)
    {
        //syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] sigwait...", __LINE__);
        printf("[thr_signal_handler:%d] sigwait...\n", __LINE__);
        err = sigwait(&signal_handler_mask, &signo);

        //syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] sigwait finished. err = %d, signo = %d", __LINE__, err, signo);
        printf("[thr_signal_handler:%d] sigwait finished. err = %d, signo = %d\n", __LINE__, err, signo);
        if (err)
        {
            //syslog(LOG_ERR, "<ERROR> [thr_signal_handler:%d] sigwait:%s", __LINE__, strerror(err));
            fprintf(stderr, "thr_signal_handler:sigwait:%s\n", strerror(err));
            fflush(stderr);
            //exit(1);
            thr_safe_exit();
            break;
        }
        switch (signo)
        {
        case SIGINT:
        case SIGTERM:
        case SIGQUIT:

            //syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d]received signal %d", __LINE__, signo);
            printf("[thr_signal_handler:%d]received signal %d\n", __LINE__, signo);
            //exit(0);
            thr_safe_exit();
            printf("[thr_signal_handler:%d] exit thr_signal_handler\n", __LINE__);
            pthread_exit(NULL);
            break;

        default:
            //syslog(LOG_ERR, "<ERROR> [thr_signal_handler:%d] unexpected signal=%d", __LINE__, signo);
            fprintf(stderr, "thr_signal_handler:unexpected signal %d\n", signo);
            //exit(1);
            thr_safe_exit();
            printf("[thr_signal_handler:%d] exit thr_signal_handler\n", __LINE__);
            pthread_exit(NULL);
        }
    }
    //syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] exit thr_signal_handler", __LINE__);
    printf("[thr_signal_handler:%d] exit thr_signal_handler\n", __LINE__);
    pthread_exit(NULL);
}
int main()
{
    sigset_t newmask;
    int err;
    pthread_t tid_signal_handler;

    pthread_mutexattr_t attr;

    struct rel_stat_st job_stat;


    atexit(main_safe_exit);

    sigemptyset(&newmask);

    sigfillset(&newmask);

    err = pthread_sigmask(SIG_BLOCK, &newmask, &mask_save);
    if (err)
    {
        fprintf(stderr, "main:pthread_sigmask():%s\n", strerror(err));
        fflush(stderr);
        exit(1);
    }
    err = pthread_create(&tid_signal_handler, NULL, thr_signal_handler, NULL);
    if (err)
    {
        fprintf(stderr, "main:pthread_create():%s\n", strerror(err));
        fflush(stderr);
        exit(1);
    }


    fd1 = open(TTY1, O_RDWR);
    if (fd1 < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd1, "TTY1\n", 5);

    fd2 = open(TTY2, O_RDWR | O_NONBLOCK);
    if (fd2 < 0)
    {
        perror("open()");
        exit(1);
    }

    write(fd2, "TTY2\n", 5);

    job1 = rel_addjob(fd1, fd2);
    if (job1 < 0)
    {
        fprintf(stderr, "[main:%d]rel_addjob()%s\n", __LINE__, strerror(-job1));
        exit(1);
    }

    printf("job1 = %d\n", job1);

    fd3 = open(TTY3, O_RDWR);
    if (fd3 < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd3, "TTY3\n", 5);

    fd4 = open(TTY4, O_RDWR | O_NONBLOCK);
    if (fd4 < 0)
    {
        perror("open()");
        exit(1);
    }

    write(fd4, "TTY4\n", 5);

    job2 = rel_addjob(fd3, fd4);
    if (job2 < 0)
    {
        fprintf(stderr, "[main:%d]rel_addjob():%s\n", __LINE__, strerror(-job2));
        exit(1);
    }

    printf("job2 = %d\n", job2);
    //menu();

    printf("\n 输入任意字符 查看job1状态：\n");
    getchar();

    err = rel_statjob(job1, &job_stat);
    if(err)
    {
        fprintf(stderr, "[main:%d]rel_statjob():%s\n", __LINE__, strerror(-err));
        exit(1);
    }
    print_jobstat(&job_stat);


    printf("\n 输入任意字符 canceljob(job2)：\n");
    getchar();
    err = rel_canceljob(job2);
    if(err)
    {
        fprintf(stderr, "[main:%d]rel_canceljob():%s\n", __LINE__, strerror(-err));
        exit(1);
    }

    printf("\n 输入任意字符 pausejob(job1)：\n");
    getchar();
    err = rel_pausejob(job1);
    if(err)
    {
        fprintf(stderr, "[main:%d]rel_pausejob():%s\n", __LINE__, strerror(-err));
        exit(1);
    }

    printf("\n 输入任意字符 resumejob(job1)：\n");   
    getchar();
    err = rel_resumejob(job1);
    if(err)
    {
        fprintf(stderr, "[main:%d]rel_resumejob():%s\n", __LINE__, strerror(-err));
        exit(1);
    }

    pthread_mutex_lock(&mut_quitflag);

    while (quitflag == 0)
    {
        //syslog(LOG_INFO, "<INFO> [main:%d]  pthread_cond_wait....", __LINE__);
        printf("[main:%d]pthread_cond_wait....\n", __LINE__);
        pthread_cond_wait(&cond_quitflag, &mut_quitflag);
        //syslog(LOG_INFO, "<INFO> [main:%d]  pthread_cond_wait finished. quitflag = %d", __LINE__, quitflag);
        printf("[main:%d]pthread_cond_wait finished. quitflag = %d\n", __LINE__, quitflag);
    }
    pthread_mutex_unlock(&mut_quitflag);

    exit(0);
}
