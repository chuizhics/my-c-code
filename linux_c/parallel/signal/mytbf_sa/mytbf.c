#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>

#include "mytbf.h"

struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
};

static struct mytbf_st *job[MYTBF_MAX];

static int inited = 0;

static timer_t timerid;

static struct sigaction osa;

/**
 * POSIX.1b 定时器发送的信号（自 Linux 2.6 起）填写 si_overrun 和 si_timerid。
 * si_timerid字段是内核用来标识定时器的内部ID；它与 timer_create(2) 返回的定时器 ID 不同。
 *  si_overrun字段是定时器溢出计数；这与调用 timer_getoverrun(2) 获得的信息相同。
 * 这些字段是非标准的 Linux 扩展。
 */
static void alrm_sigaction(int sig, siginfo_t *info, void *unused)
{

    int i = 0;
    syslog(LOG_INFO,"[%s:%d]:received signal %d", __FUNCTION__, __LINE__, sig);
    syslog(LOG_INFO,"[%s:%d]:info->si_code = %d,info->signo = %d", __FUNCTION__, __LINE__, info->si_code, info->si_signo);

    if (info->si_code != SI_TIMER)
    {
        return;
    }

    for (i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if (job[i]->token > job[i]->burst)
            {
                job[i]->token = job[i]->burst;
            }
        }
    }
}

static void module_unload(void)
{
    int i;

    syslog(LOG_INFO,"enter module_unload()");
    if (inited)
    {
        sigaction(SIGALRM, &osa, NULL);
        timer_delete(&timerid);
        closelog();
    }
    for (i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            free(job[i]);
        }
    }
}

static void module_load(void)
{
    int timer_settime_ret;

    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec its;

    openlog("mytbf_sa", LOG_PID, LOG_DAEMON);

    // 只输出 LOG_INFO 及以下级别的日志
    setlogmask(LOG_UPTO(LOG_INFO));

    syslog(LOG_INFO, "[%s:%d]:enter module_load()", __FUNCTION__, __LINE__);

    // 注册信号处理函数
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = alrm_sigaction;

    sigaction(SIGALRM, &sa, &osa);

    // 设置定时器事件
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;

    // 创建定时器
    timer_create(CLOCK_REALTIME, &sev, &timerid);

    // 设置定时器
    its.it_value.tv_sec = 1; // 初次到期时间
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1; // 重复间隔时间
    its.it_interval.tv_nsec = 0;

    timer_settime_ret = timer_settime(timerid, 0, &its, NULL);

    atexit(module_unload);
}

static int get_free_pos(void)
{
    int i = 0;
    for (i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

mytbf_t *mytbf_init(int cps, int burst)
{

    struct mytbf_st *tbf = NULL;
    int pos = 0;

    if (!inited)
    {
        module_load();
        inited = 1;
    }

    pos = get_free_pos();
    if (pos < 0)
    {
        return NULL;
    }

    tbf = malloc(sizeof(struct mytbf_st));
    if (tbf == NULL)
    {
        return NULL;
    }

    tbf->token = 0;
    tbf->cps = cps;
    tbf->burst = burst;
    tbf->pos = pos;

    job[pos] = tbf;

    return tbf;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
    syslog(LOG_INFO, "[%s:%d]:enter mytbf_fetchtoken()", __FUNCTION__, __LINE__);
    struct mytbf_st *tbf = ptr;
    int ret = 0;

    if (size <= 0)
    {
        return -EINVAL;
    }

    syslog(LOG_INFO, "[%s:%d]:tbf->token = %d", __FUNCTION__, __LINE__, tbf->token);

    while (tbf->token <= 0)
    {
        pause();
    }
    syslog(LOG_INFO, "[%s:%d]:tbf->token = %d", __FUNCTION__, __LINE__, tbf->token);

    ret = tbf->token > size ? size : tbf->token;

    tbf->token -= ret;

    return ret;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *tbf = ptr;

    if (size <= 0)
    {
        return -EINVAL;
    }

    tbf->token += size;
    if (tbf->token > tbf->burst)
        tbf->token = tbf->burst;

    return size;
}

int mytbf_destroy(mytbf_t *ptr)
{
    syslog(LOG_INFO, "[%s:%d]:enter mytbf_destroy()", __FUNCTION__, __LINE__);
    struct mytbf_st *tbf = ptr;
    job[tbf->pos] = NULL;
    free(tbf);

    return 0;
}
