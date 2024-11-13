#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "anytimer.h"

enum job_status
{
    WAITING, /*等待运行*/
    RUNNING, /*正在运行*/
    PAUSE,   /*暂停*/
    OVER,    /*执行完成*/
    CANCEL   /*已取消*/
};

struct job_st
{
    int sec;
    at_jobfun_t *jobf;
    void *arg;
    enum job_status status;
};

static struct job_st *job[JOB_MAX];

static int inited = 0;

static void (*alrm_handler_save)(int);

static void alrm_handler(int sig)
{
    int i = 0;
    alarm(1);
    for (i = 0; i < JOB_MAX; i++)
    {
        if (job[i] != NULL && job[i]->status == WAITING)
        {
            if (job[i]->sec > 0)
            {
                job[i]->sec--;
            }
            else
            {
                job[i]->status = RUNNING;
                job[i]->jobf(job[i]->arg);
                job[i]->status = OVER;
            }
        }
    }
}

static void module_unload(void)
{
    int i;
    signal(SIGALRM, alrm_handler_save);
    alarm(0);
    for (i = 0; i < JOB_MAX; i++)
    {
        if (job[i] != NULL)
        {
            free(job[i]);
            job[i] = NULL;
        }
    }
}

static void module_load(void)
{
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);

    atexit(module_unload);
}

static int get_free_pos(void)
{
    int i = 0;
    for (i = 0; i < JOB_MAX; i++)
    {
        if (job[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

/**
 * return >= 0          成功，返回任务id
 *        == -EINVAL    失败，参数非法
 *        == -ENOSPC    失败，数组满
 *        == -ENOMEM    失败，内存空间不足
 *
 */
int at_addjob(int sec, at_jobfun_t *jobp, void *arg)
{
    struct job_st *job_ptr = NULL;
    int pos = -1;

    if(sec < 0 || jobp == NULL)
    {
        return -EINVAL;
    }

    if (!inited)
    {
        module_load();
        inited = 1;
    }

    pos = get_free_pos();
    if (pos < 0)
    {
        return -ENOSPC;
    }

    job_ptr = malloc(sizeof(struct job_st));    
    if (job_ptr == NULL)
    {
        return -ENOMEM;
    }
    
    job_ptr->sec = sec;
    job_ptr->jobf = jobp;
    job_ptr->arg = arg;
    job_ptr->status = WAITING;

    job[pos] = job_ptr;

    return pos;
}

/**
 * return  == 0         成功，指定任务成功取消
 *         == -EINVAL   失败，参数非法
 *         == -EBUSY    失败，指定任务正在运行或已完成
 *         == -ECANCELED 失败，指定任务重复取消
 *
 */
int at_canceljob(int id)
{
    if(id < 0 || id > JOB_MAX || job[id] == NULL)
    {
        return -EINVAL;
    }

    switch (job[id]->status)
    {
        case WAITING:
            job[id]->status = CANCEL;
            break;
        case RUNNING:
            return -EBUSY;
        case PAUSE:
            job[id]->status = CANCEL;
        case CANCEL:
            return -ECANCELED;
        case OVER:
            return -EBUSY;
        default:
            break;
    }
    return 0;
}

/**
 * return  == 0         成功，指定任务成功暂停
 *         == -EINVAL   失败，参数非法
 *         == -EBUSY    失败，指定任务已完成
 *         == -ECANCELED 失败，指定任务已取消
 *
 */
int at_pausejob(int id)
{
    return 0;
}

/**
 * return == 0          成功，指定任务已恢复
 *        == -EINVAL    失败，参数非法
 *        == -EBUSY     失败，指定任务正在执行或已完成
 *        == -ECANCELED 失败，指定任务已取消
 *
 */
int at_resumejob(int id)
{
    return 0;
}

/**
 * return == 0          成功，指定任务成功释放
 *        == -EINVAL    失败，参数非法
 *
 */
int at_waitjob(int id)
{
    if(id < 0 || id >= JOB_MAX)
    {
        return -EINVAL;
    }
    if(job[id] != NULL)
    {
        free(job[id]);
        job[id] = NULL;
    }
    return 0;
}
