#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <syslog.h>

#include <pthread.h>

#include "mytbf.h"

struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mut;
};

static pthread_once_t pth_once = PTHREAD_ONCE_INIT;

static struct mytbf_st *job[MYTBF_MAX];

static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;

//static volatile int inited = 0;

//tatic pthread_mutex_t mut_inited = PTHREAD_MUTEX_INITIALIZER;

static pthread_t tid;

static void *thr_alrm(void *arg)
{

    int i = 0;
    // syslog(LOG_INFO,"[%s:%d]:received signal %d", __FUNCTION__, __LINE__, sig);
    // syslog(LOG_INFO,"[%s:%d]:info->si_code = %d,info->signo = %d", __FUNCTION__, __LINE__, info->si_code, info->si_signo);

    while (1)
    {
        pthread_mutex_lock(&mut_job);
        for (i = 0; i < MYTBF_MAX; i++)
        {
            if (job[i] != NULL)
            {
                pthread_mutex_lock(&job[i]->mut);

                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst)
                {
                    job[i]->token = job[i]->burst;
                }
                pthread_mutex_unlock(&job[i]->mut);
            }
        }
        pthread_mutex_unlock(&mut_job);
        
        sleep(1);
    }
}

static void module_unload(void)
{
    int i;

    syslog(LOG_INFO, "<info> enter module_unload()...");
    


    pthread_cancel(tid);
    pthread_join(tid, NULL);

    for (i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            mytbf_destroy(job[i]);
        }
    }

    // if (inited)
    // {
    //     closelog();
    // }
    // pthread_mutex_destroy(&mut_inited);

    pthread_mutex_destroy(&mut_job);

    closelog();
}

static void module_load(void)
{
    int err;
    openlog("mytbf_sa", LOG_PID, LOG_DAEMON);

    // 只输出 LOG_INFO 及以下级别的日志
    setlogmask(LOG_UPTO(LOG_INFO));

    syslog(LOG_INFO, "<info> [%s:%d]:enter module_load()", __FUNCTION__, __LINE__);

    err = pthread_create(&tid,NULL,thr_alrm,NULL);
    if (err)
    {
        fprintf(stderr,"%s\n",strerror(err));
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

    // pthread_mutex_lock(&mut_inited);
    // if (!inited)
    // {
    //     module_load();
    //     inited = 1;
    // }
    // pthread_mutex_unlock(&mut_inited);

    pthread_once(&pth_once, module_load);


    tbf = malloc(sizeof(struct mytbf_st));
    if (tbf == NULL)
    {
        return NULL;
    }

    tbf->token = 0;
    tbf->cps = cps;
    tbf->burst = burst;
    pthread_mutex_init(&tbf->mut, NULL);

    pthread_mutex_lock(&mut_job);

    pos = get_free_pos_unlocked();
    if (pos < 0)
    {
        pthread_mutex_unlock(&mut_job);
        return NULL;
    }
    tbf->pos = pos;
    job[pos] = tbf;
    pthread_mutex_unlock(&mut_job);

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
        pthread_mutex_unlock(&tbf->mut);
        sched_yield();
        pthread_mutex_lock(&tbf->mut);
    }
    syslog(LOG_INFO, "<info> [%s:%d]:tbf->token = %d", __FUNCTION__, __LINE__, tbf->token);

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
        tbf->token = tbf->burst;
    pthread_mutex_unlock(&tbf->mut);

    return size;
}

int mytbf_destroy(mytbf_t *ptr)
{
    syslog(LOG_INFO, "<info> [%s:%d]:enter mytbf_destroy()", __FUNCTION__, __LINE__);
    struct mytbf_st *tbf = ptr;

    if(tbf != NULL)
    {
        pthread_mutex_lock(&mut_job);
        job[tbf->pos] = NULL;
        pthread_mutex_unlock(&mut_job);
        
        pthread_mutex_destroy(&tbf->mut);
        free(tbf);
        tbf = NULL;
    }

    return 0;
}
