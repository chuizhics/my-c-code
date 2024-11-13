#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#include <syslog.h>

#include <pthread.h>

#include "mytbf.h"

#define LOGNAME "mytbf_mt_notify"

struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};

static pthread_once_t pth_once = PTHREAD_ONCE_INIT;

static struct mytbf_st *job[MYTBF_MAX];

static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
// static pthread_cond_t cond_job = PTHREAD_COND_INITIALIZER;

static pthread_t tid;

static void *thr_alrm(void *arg)
{

    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;
    int i = 0;
    int flag = 0;

    while (1)
    {
        pthread_testcancel();
        pthread_mutex_lock(&mut_job);
        for (i = 0; i < MYTBF_MAX; i++)
        {
            if (job[i] != NULL)
            {
                syslog(LOG_INFO, "<info> [%s:%d]:pthread_mutex_lock(&job[i]->mut)..", __FUNCTION__, __LINE__);
                pthread_mutex_lock(&job[i]->mut);

                // if (job[i]->token == job[i]->burst + 1)
                // { // 取消信号来临
                //     syslog(LOG_INFO, "<info> [%s:%d]:received cancel signal", __FUNCTION__, __LINE__);
                //     // flag = 1;
                //     pthread_mutex_unlock(&job[i]->mut);
                //     break;
                // }

                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst)
                {
                    job[i]->token = job[i]->burst;
                }
                pthread_cond_broadcast(&job[i]->cond);
                pthread_mutex_unlock(&job[i]->mut);
                syslog(LOG_INFO, "<info> [%s:%d]:pthread_mutex_unlock(&job[i]->mut)..", __FUNCTION__, __LINE__);
            }
        }
        pthread_mutex_unlock(&mut_job);

        //nanosleep(&ts, NULL);
    }
    pthread_exit(NULL);
}

static void module_unload(void)
{
    int i;

    syslog(LOG_INFO, "<info> [%s:%d]:enter module_unload()", __FUNCTION__, __LINE__);

    pthread_cancel(tid);
    
    pthread_join(tid, NULL);

    for (i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            mytbf_destroy(job[i]);
        }
    }

   // pthread_cond_destroy(&cond_job);
    pthread_mutex_destroy(&mut_job);

    closelog();
}

static void module_load(void)
{
    int err;
    openlog(LOGNAME, LOG_PID, LOG_DAEMON);

    // 只输出 LOG_INFO 及以下级别的日志
    setlogmask(LOG_UPTO(LOG_INFO));

    syslog(LOG_INFO, "<info> [%s:%d]:enter module_load()", __FUNCTION__, __LINE__);

    err = pthread_create(&tid, NULL, thr_alrm, NULL);
    if (err)
    {
        fprintf(stderr, "%s\n", strerror(err));
    }

    atexit(module_unload);
}

static int get_free_pos_unlocked(void)
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

    pthread_once(&pth_once, module_load);

    tbf = malloc(sizeof(struct mytbf_st));
    if (tbf == NULL)
    {
        syslog(LOG_ERR, "<error> [%s:%d]:malloc():%s", __FUNCTION__, __LINE__, strerror(errno));
        return NULL;
    }

    tbf->token = 0;
    tbf->cps = cps;
    tbf->burst = burst;
    pthread_mutex_init(&tbf->mut, NULL);
    pthread_cond_init(&tbf->cond, NULL);

    pthread_mutex_lock(&mut_job);

    pos = get_free_pos_unlocked();
    if (pos < 0)
    {
        syslog(LOG_ERR, "<error> [%s:%d]:get_free_pos_unlocked():%s", __FUNCTION__, __LINE__, strerror(ENOSPC));
        pthread_mutex_unlock(&mut_job);
        return NULL;
    }
    tbf->pos = pos;
    job[pos] = tbf;
    pthread_mutex_unlock(&mut_job);

    syslog(LOG_INFO, "<info> [%s:%d]:init finished.", __FUNCTION__, __LINE__);
    return tbf;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
    syslog(LOG_INFO, "<info> [%s:%d]:enter mytbf_fetchtoken()", __FUNCTION__, __LINE__);
    struct mytbf_st *tbf = ptr;
    int ret = 0;
    int token = 0;

    if (size <= 0)
    {
        return -EINVAL;
    }

    pthread_mutex_lock(&tbf->mut);
    syslog(LOG_INFO, "<info> [%s:%d]:tbf->token = %d", __FUNCTION__, __LINE__, tbf->token);

    while (tbf->token <= 0)
    {

        syslog(LOG_INFO, "<info> [%s:%d] pthread_cond_wait(&tbf->cond, &tbf->mut)....", __FUNCTION__, __LINE__);

        // 加入等待队列 --> 解锁tbf->mut --->等待唤醒 ---> 出队再抢锁
        /*enqueue-->unlock-->wait() -->lock*/
        pthread_cond_wait(&tbf->cond, &tbf->mut);
    }

    syslog(LOG_INFO, "<info> [%s:%d]:tbf->token = %d", __FUNCTION__, __LINE__, tbf->token);

    // if (tbf->token == tbf->burst + 1)
    // { // 取消信号来临
    //     syslog(LOG_INFO, "<info> [%s:%d]:received cancel signal", __FUNCTION__, __LINE__);
    //     pthread_mutex_unlock(&tbf->mut);
    //     return -EFAULT;
    // }

    ret = tbf->token > size ? size : tbf->token;

    tbf->token -= ret;

    pthread_mutex_unlock(&tbf->mut);

    return ret;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *tbf = ptr;

    if (size <= 0)
    {
        return -EINVAL;
    }

    pthread_mutex_lock(&tbf->mut);
    tbf->token += size;
    if (tbf->token > tbf->burst)
    {
        tbf->token = tbf->burst;
    }

    pthread_cond_broadcast(&tbf->cond);

    pthread_mutex_unlock(&tbf->mut);

    return size;
}

int mytbf_destroy(mytbf_t *ptr)
{
    syslog(LOG_INFO, "<info> [%s:%d]:enter mytbf_destroy()", __FUNCTION__, __LINE__);
    struct mytbf_st *tbf = ptr;

    if (tbf != NULL)
    {
        pthread_mutex_lock(&mut_job);
        job[tbf->pos] = NULL;
        pthread_mutex_unlock(&mut_job);

        pthread_mutex_lock(&tbf->mut);

        // // 发送退出信号
        // syslog(LOG_INFO, "<info> [%s:%d]:send cancel signal", __FUNCTION__, __LINE__);
        // tbf->token = tbf->burst + 1;

        pthread_cond_broadcast(&tbf->cond);
        pthread_mutex_unlock(&tbf->mut);

        if (0 != tbf->cond.__data.__wrefs)
        {
            syslog(LOG_ERR, "<error> [%s:%d]:tbf->cond.__data.__wrefs = %u", __FUNCTION__, __LINE__, tbf->cond.__data.__wrefs);
            tbf->cond.__data.__wrefs = 0;
        }

        pthread_cond_destroy(&tbf->cond);

        syslog(LOG_INFO, "<info> [%s:%d]:pthread_cond_destroy(&tbf->cond) finished.", __FUNCTION__, __LINE__);
        pthread_mutex_destroy(&tbf->mut);

        free(tbf);
    }
    syslog(LOG_INFO, "<info> [%s:%d]:mytbf_destroy() finished.", __FUNCTION__, __LINE__);
    return 0;
}
