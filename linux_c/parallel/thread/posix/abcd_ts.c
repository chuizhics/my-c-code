#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <time.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#define THRNUM 4

#define LOGNAME "abcd_ts"

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static pthread_t tid[THRNUM];

static pthread_t tid_s;

static sigset_t mask;

static int quitflag = 0; /*set nonzero by thread*/
static pthread_mutex_t mut_quitflag = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_quitflag = PTHREAD_COND_INITIALIZER;

static int next(int n)
{
    if (n == THRNUM - 1)
    {
        return 0;
    }
    return n + 1;
}

static void *thr_func(void *arg)
{
    syslog(LOG_INFO, "<INFO> [thr_func%d:%d]  enter thr_func", (int)arg, __LINE__);
    // printf("[%d] enter thr_func\n", __LINE__);
    int n = (int)arg;
    int ch = 'a' + n;

    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;
    while (1)
    {
        pthread_mutex_lock(&mut_num);

        while (num != n && num != -1)
        {
            syslog(LOG_INFO, "<INFO> [thr_func%d:%d]  pthread_cond_wait....", n, __LINE__);
            pthread_cond_wait(&cond_num, &mut_num);
            syslog(LOG_INFO, "<INFO> [thr_func%d:%d]  pthread_cond_wait finished.", n, __LINE__);
        }
        if (num == -1)
        {
            pthread_mutex_unlock(&mut_num);
            break;
        }
        write(1, &ch, 1);
        // fprintf(stdout,"%c", ch);
        // fflush(stdout);
        num = next(num);
        pthread_cond_broadcast(&cond_num);
        pthread_mutex_unlock(&mut_num);
        nanosleep(&ts, NULL);
    }

    syslog(LOG_INFO, "<INFO> [thr_func%d:%d]  exit thr_func", n, __LINE__);
    pthread_exit(NULL);
}

static void _safe_exit(void)
{
    // fprintf(stdout, "received signal %d\n", signo);
    pthread_mutex_lock(&mut_num);

    num = -1; // 向除主线程外的其余线程发送退出信号
    pthread_cond_broadcast(&cond_num);

    pthread_mutex_unlock(&mut_num);

    pthread_mutex_lock(&mut_quitflag);

    quitflag = 1; // 向主线程发送退出信号
    pthread_cond_broadcast(&cond_quitflag);

    pthread_mutex_unlock(&mut_quitflag);

}

static void *thr_signal_handler(void *arg)
{
    int i, err, signo;
    syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] enter thr_signal_handler", __LINE__);
    // printf("[%d] enter thr_signal_handler\n", __LINE__);
    sigset_t alrm_handler_mask;


    sigemptyset(&alrm_handler_mask);
    sigaddset(&alrm_handler_mask, SIGALRM);
    sigaddset(&alrm_handler_mask, SIGINT);
    sigaddset(&alrm_handler_mask, SIGTERM);
    sigaddset(&alrm_handler_mask, SIGQUIT);

    // False  pthread_sigmask(SIG_UNBLOCK, &mask, NULL);

    while (1)
    {
        syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] sigwait...", __LINE__);

        /*The signals defined by set shall have been blocked at the time of the call to sigwait();
            otherwise, the behavior is undefined. 
            set 定义的信号在调用 sigwait() 时应该已经被阻塞；
        */
        err = sigwait(&alrm_handler_mask, &signo);

        syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] sigwait finished. err = %d, signo = %d", __LINE__, err, signo);
        if (err)
        {
            syslog(LOG_ERR, "<ERROR> [thr_signal_handler:%d] sigwait:%s", __LINE__, strerror(err));
            // fprintf(stderr, "thr_signal_handler:sigwait:%s\n", strerror(err));
            // fflush(stderr);
            //exit(1);
            _safe_exit();
            break;
        }
        switch (signo)
        {
        case SIGALRM:
        case SIGINT:
        case SIGTERM:
        case SIGQUIT:

            syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d]received signal %d", __LINE__, signo);

            _safe_exit();
            break;

        default:
            syslog(LOG_ERR, "<ERROR> [thr_signal_handler:%d] unexpected signal=%d", __LINE__, signo);
            // fprintf(stderr, "thr_signal_handler:unexpected signal %d\n", signo);
            exit(1);
        }
    }
    syslog(LOG_INFO, "<INFO> [thr_signal_handler:%d] exit thr_signal_handler", __LINE__);
    pthread_exit(NULL);
}

int main()
{
    openlog(LOGNAME, LOG_PID, LOG_DAEMON);
    int i, err;
    sigset_t oldmask;

    sigemptyset(&mask);

    sigfillset(&mask);
    // sigaddset(&mask, SIGALRM);
    // sigaddset(&mask, SIGINT);
    // sigaddset(&mask, SIGTERM);

    // 不能被忽略处理的信号SIGKILL 和SIGSTOP无法被阻塞。
    //  F sigaddset(&mask, SIGSTOP);
    // 经测试，猜测（存疑，很不确定） 下行代码会使线程的屏蔽字（mask）直接使用（或者说替换成）了mask引用。从而对变量mask的修改会直接影响线程的屏蔽字
    // 
    err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask);
    if (err)
    {
        fprintf(stderr, "main:pthread_sigmask():%s\n", strerror(err));
        fflush(stderr);
        exit(1);
    }

    err = pthread_create(&tid_s, NULL, thr_signal_handler, NULL);
    if (err)
    {
        fprintf(stderr, "main:pthread_create():%s\n", strerror(err));
        fflush(stderr);
        exit(1);
    }

    for (i = 0; i < THRNUM; i++)
    {
        // 当一个线程调用pthread_create() 创建新的线程时，此线程的信号掩码会被新创建的线程继承。
        err = pthread_create(tid + i, NULL, thr_func, (void *)i);
        if (err)
        {
            fprintf(stderr, "main:pthread_create():%s\n", strerror(err));
            fflush(stderr);
            exit(1);
        }
    }

    alarm(100);

    pthread_mutex_lock(&mut_quitflag);

    while (quitflag == 0)
    {
        syslog(LOG_INFO, "<INFO> [main:%d]  pthread_cond_wait....", __LINE__);
        pthread_cond_wait(&cond_quitflag, &mut_quitflag);
        syslog(LOG_INFO, "<INFO> [main:%d]  pthread_cond_wait finished. quitflag = %d", __LINE__, quitflag);

    }
    pthread_mutex_unlock(&mut_quitflag);


    for (i = 0; i < THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_cond_destroy(&cond_quitflag);

    pthread_mutex_destroy(&mut_quitflag);

    pthread_cond_destroy(&cond_num);

    pthread_mutex_destroy(&mut_num);

    syslog(LOG_INFO, "[main:%d] exit main", __LINE__);
    closelog();

    //sigpending();

    /** 恢复旧的mask之前，传递过来的信号
     *   要么是被处理了（SIGALRM、SIGINT、SIGTERM、SIGQUIT）
     *   要么是被线程阻塞了而处于挂起（pending态） --》 表现在进程的pending表中
     *    所以如果直接恢复旧的mask状态，会导致之前传递的未处理的信号，会被立即处理。
     *          ！！！ 所以要在清理完成之后再恢复mask,否则会可能导致清理工作无法完成
     *    所以，在 pthread_sigmask(SIG_BLOCK, &mask, &oldmask);
     *         和 pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
     *         之间的代码有点lock、unlock的感觉。
    */
    err = pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
    if (err)
    {
        fprintf(stderr, "main:pthread_sigmask():%s\n", strerror(err));
        fflush(stderr);
        exit(1);
    }

    exit(0);
}