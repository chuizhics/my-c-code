#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <time.h>

#include <string.h>
#include <pthread.h>

#include <syslog.h>

#include "relayer.h"

#define BUFSIZE 1024

#define LOGNAME "relayer"

enum
{
    STATE_R = 1,
    STATE_W,
    STATE_Ex,
    STATE_T
};

struct rel_fsm_st
{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int nread;
    int pos;
    char *errstr;
    int64_t count;

    // pthread_mutex_t mut_fsm_state;
};

struct rel_job_st
{
    int job_id;
    int job_state;
    int fd1;
    int fd1_save;
    int fd2;
    int fd2_save;
    struct rel_fsm_st fsm12;
    struct rel_fsm_st fsm21;
    struct timespec time_start;
    struct timespec time_end;
    // pthread_mutex_t mut_job_state;
};

static struct rel_job_st *rel_job[REL_JOBMAX];
static pthread_mutex_t mut_rel_job;
static pthread_mutexattr_t mutattr_rel_job;

static pthread_once_t init_once = PTHREAD_ONCE_INIT;

static pthread_t tid_relayer;

static int rel_waitjob_unlocked(int, struct rel_stat_st *);

static void fsm_driver(struct rel_fsm_st *fsm)
{
    // syslog(LOG_INFO, "<INFO> [%s:%d]:enter fsm_driver()", __FUNCTION__, __LINE__);
    int nwrite;

    if (fsm != NULL)
    {
        // pthread_mutex_lock(&fsm->mut_fsm_state);
        switch (fsm->state)
        {
        case STATE_R:
            fsm->nread = read(fsm->sfd, fsm->buf, BUFSIZE);
            if (fsm->nread == 0)
            {
                fsm->state = STATE_T;
            }
            else if (fsm->nread < 0)
            {
                if (errno == EAGAIN)
                {
                    fsm->state = STATE_R;
                }
                else
                {
                    fsm->errstr = "read()";
                    fsm->state = STATE_Ex;
                }
            }
            else
            {
                fsm->pos = 0;
                fsm->state = STATE_W;
            }

            break;
        case STATE_W:
            nwrite = write(fsm->dfd, fsm->buf + fsm->pos, fsm->nread);
            if (nwrite < 0)
            {
                if (errno == EAGAIN)
                {
                    fsm->state = STATE_W;
                }
                else
                {
                    fsm->errstr = "write()";
                    fsm->state = STATE_Ex;
                }
            }
            else
            {
                fsm->pos += nwrite;
                fsm->nread -= nwrite;
                fsm->count += nwrite;
                if (fsm->nread == 0)
                {
                    fsm->state = STATE_R;
                }
                else
                {
                    fsm->state = STATE_W;
                }
            }

            break;

        case STATE_Ex:
            perror(fsm->errstr);
            fsm->state = STATE_T;
            break;
        case STATE_T:
            /*do sth*/
            break;
        default:
            // pthread_mutex_lock(&fsm->mut_fsm_state);
            abort();
            break;
        }
        // pthread_mutex_lock(&fsm->mut_fsm_state);
    }

    // syslog(LOG_INFO, "<INFO> [%s:%d]:exit fsm_driver()", __FUNCTION__, __LINE__);
}

static int get_free_pos_unlocked(void)
{
    int i = 0;
    for (i = 0; i < REL_JOBMAX; i++)
    {
        if (rel_job[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

static void *thr_relayer(void *arg)
{
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter thr_relayer()", __FUNCTION__, __LINE__);
    int i = 0;
    // pthread_cleanup_push();  

    while (1)
    {
        pthread_testcancel();
        pthread_mutex_lock(&mut_rel_job);
        for (i = 0; i < REL_JOBMAX; i++)
        {
            if (rel_job[i] != NULL)
            {
                // pthread_mutex_lock(&(rel_job[i]->mut_job_state));
                if (rel_job[i]->job_state == STATE_RUNNING)
                {
                    clock_gettime(CLOCK_REALTIME, &rel_job[i]->time_end);
                    fsm_driver(&rel_job[i]->fsm12);
                    fsm_driver(&rel_job[i]->fsm21);
                    // pthread_mutex_lock(&(rel_job[i]->fsm12.mut_fsm_state));

                    // pthread_mutex_lock(&(rel_job[i]->fsm21.mut_fsm_state));
                    if (rel_job[i]->fsm12.state == STATE_T && rel_job[i]->fsm21.state == STATE_T)
                    {
                        rel_job[i]->job_state = STATE_OVER;
                    }
                    // pthread_mutex_unlock(&(rel_job[i]->fsm21.mut_fsm_state));

                    // pthread_mutex_unlock(&(rel_job[i]->fsm12.mut_fsm_state));
                }
                // pthread_mutex_unlock(&(rel_job[i]->mut_job_state));
            }
        }
        pthread_mutex_unlock(&mut_rel_job);
        sched_yield();
    }
    syslog(LOG_INFO, "<INFO> [%s:%d]:exit thr_relayer()", __FUNCTION__, __LINE__);
    pthread_exit(NULL);
}

static void module_unload(void)
{
    int i, s;

    syslog(LOG_INFO, "<INFO> [%s:%d]:enter module_unload()", __FUNCTION__, __LINE__);

    syslog(LOG_INFO, "<INFO> [%s:%d]:pthread_cancel(tid_relayer)", __FUNCTION__, __LINE__);
    s = pthread_cancel(tid_relayer);
    if (s)
    {
        fprintf(stderr, "pthread_cancel():%s\n", strerror(s));
        _exit(1);
    }


    syslog(LOG_INFO, "<INFO> [%s:%d]:pthread_cancel(tid_relayer) finished", __FUNCTION__, __LINE__);
    pthread_join(tid_relayer, NULL);
    syslog(LOG_INFO, "<INFO> [%s:%d]:pthread_join(tid_relayer, NULL) finished", __FUNCTION__, __LINE__);

    s = pthread_mutex_lock(&mut_rel_job);

    if (s == EOWNERDEAD)
    {
        syslog(LOG_INFO, "<INFO> [%s:%d]:pthread_mutex_lock() returned EOWNERDEAD", __FUNCTION__, __LINE__);
        // printf("[main] pthread_mutex_lock() returned EOWNERDEAD\n");

        syslog(LOG_INFO, "<INFO> [%s:%d]:Now make the mutex consistent", __FUNCTION__, __LINE__);
        // printf("[main] Now make the mutex consistent\n");
        s = pthread_mutex_consistent(&mut_rel_job);
        if (s)
        {
            fprintf(stderr, "pthread_mutex_consistent():%s\n", strerror(s));
            pthread_mutex_unlock(&mut_rel_job);
            pthread_mutex_destroy(&mut_rel_job);

            pthread_mutex_init(&mut_rel_job, NULL);
            pthread_mutex_lock(&mut_rel_job);
            // _exit(1);
        }
        syslog(LOG_INFO, "<INFO> [%s:%d]:Mutex is now consistent", __FUNCTION__, __LINE__);
    }
    else if (s == 0)
    {
        // do nothing
        // printf("[main] pthread_mutex_lock() unexpectedly succeeded\n");
    }
    else
    {
        syslog(LOG_ERR, "<ERROR> [%s:%d]:pthread_mutex_lock() unexpectedly failed", __FUNCTION__, __LINE__);
        fprintf(stderr, "pthread_mutex_lock():%s\n", strerror(s));
        _exit(1);
        // handle_error_en(s, "pthread_mutex_lock");
    }
    for (i = 0; i < REL_JOBMAX; i++)
    {
        if (rel_job[i] != NULL)
        {
            // rel_waitjob(rel_job[i]->job_id, NULL); 死锁
            // rel_waitjob_unlocked(rel_job[i]->job_id, NULL);
            fcntl(rel_job[i]->fd2, F_SETFL, rel_job[i]->fd2_save);
            fcntl(rel_job[i]->fd1, F_SETFL, rel_job[i]->fd1_save);
            free(rel_job[i]);
            rel_job[i] = NULL;
        }
    }
    pthread_mutex_unlock(&mut_rel_job);

    pthread_mutex_destroy(&mut_rel_job);

    pthread_mutexattr_destroy(&mutattr_rel_job);

    syslog(LOG_INFO, "<INFO> [%s:%d]:exit module_unload()", __FUNCTION__, __LINE__);
    closelog();
}

static void module_load(void)
{
    int err;

    atexit(module_unload);

    openlog(LOGNAME, LOG_PID, LOG_DAEMON);
    // 只输出 LOG_INFO 及以下级别的日志
    setlogmask(LOG_UPTO(LOG_INFO));

    syslog(LOG_INFO, "<INFO> [%s:%d]:enter module_load()", __FUNCTION__, __LINE__);

    pthread_mutexattr_init(&mutattr_rel_job);

    pthread_mutexattr_setrobust(&mutattr_rel_job, PTHREAD_MUTEX_ROBUST);

    pthread_mutex_init(&mut_rel_job, &mutattr_rel_job);

    err = pthread_create(&tid_relayer, NULL, thr_relayer, NULL);
    if (err)
    {
        fprintf(stderr, "[module_load:%d] pthread_create(): %s\n", __LINE__, strerror(err));
        exit(1);
    }
    syslog(LOG_INFO, "<INFO> [%s:%d]:exit module_load()", __FUNCTION__, __LINE__);
}

/**
 * return   >= 0            成功，返回当前任务ID
 *          == -EINVAL      失败，参数非法
 *          == -ENOSPC      失败，任务数组满
 *          == -ENOMEM      失败，内存分配有误
 */
int rel_addjob(int fd1, int fd2)
{
    struct rel_job_st *me = NULL;

    pthread_once(&init_once, module_load);
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter rel_addjob()", __FUNCTION__, __LINE__);

    me = malloc(sizeof(*me));
    if (me == NULL)
    {
        return -ENOMEM;
    }
    me->fd1 = fd1;
    me->fd2 = fd2;
    me->job_state = STATE_RUNNING;
    clock_gettime(CLOCK_REALTIME, &me->time_start);

    clock_gettime(CLOCK_REALTIME, &me->time_end);

    me->fd1_save = fcntl(me->fd1, F_GETFL);
    fcntl(me->fd1, F_SETFL, me->fd1_save | O_NONBLOCK);

    me->fd2_save = fcntl(me->fd2, F_GETFL);
    fcntl(me->fd2, F_SETFL, me->fd2_save | O_NONBLOCK);

    me->fsm12.sfd = me->fd1;
    me->fsm12.dfd = me->fd2;
    me->fsm12.state = STATE_R;
    me->fsm12.count = 0;
    // pthread_mutex_init(&me->fsm12.mut_fsm_state, NULL);

    me->fsm21.sfd = me->fd2;
    me->fsm21.dfd = me->fd1;
    me->fsm21.state = STATE_R;
    me->fsm21.count = 0;
    // pthread_mutex_init(&me->fsm21.mut_fsm_state, NULL);

    // pthread_mutex_init(&me->mut_job_state, NULL);

    pthread_mutex_lock(&mut_rel_job);
    me->job_id = get_free_pos_unlocked();
    if (me->job_id < 0)
    {
        pthread_mutex_unlock(&mut_rel_job);
        fcntl(me->fd2, F_SETFL, me->fd2_save);
        fcntl(me->fd1, F_SETFL, me->fd1_save);
        free(me);
        return -ENOSPC;
    }

    rel_job[me->job_id] = me;
    pthread_mutex_unlock(&mut_rel_job);

    syslog(LOG_INFO, "<INFO> [%s:%d]:exit rel_addjob()", __FUNCTION__, __LINE__);
    return me->job_id;
}

/**
 * return  == 0             成功，指定任务成功取消（包括重复取消）
 *         == -EINVAL       失败，参数非法
 *         //== -EOWNERDEAD   失败，指定任务已完成
 *         // == -ECANCELED    失败，指定任务重复取消
 *         == -ECANCELD     失败，指定任务无法取消（已完成）
 */
int rel_canceljob(int id)
{
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter rel_canceljob()", __FUNCTION__, __LINE__);
    if (id < 0 || id > REL_JOBMAX)
    {
        return -EINVAL;
    }
    pthread_mutex_lock(&mut_rel_job);
    if (rel_job[id] != NULL)
    {
        if (rel_job[id]->job_state != STATE_CANCELED)
        {
            if (rel_job[id]->job_state == STATE_OVER)
            {
                pthread_mutex_unlock(&mut_rel_job);
                return -ECANCELED;
            }
            clock_gettime(CLOCK_REALTIME, &rel_job[id]->time_end);
            rel_job[id]->job_state = STATE_CANCELED;
        }
    }
    pthread_mutex_unlock(&mut_rel_job);
    syslog(LOG_INFO, "<INFO> [%s:%d]:exit rel_canceljob()", __FUNCTION__, __LINE__);
    return 0;
}

/**
 * return  == 0             成功，任务成功暂停
 *         == -EINVAL       失败，参数非法
 *         == -ECANCLED     失败，指定任务无法暂停（已完成或者是CANCELED）
 */
int rel_pausejob(int id)
{
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter rel_pausejob()", __FUNCTION__, __LINE__);
    if (id < 0 || id > REL_JOBMAX)
    {
        return -EINVAL;
    }
    pthread_mutex_lock(&mut_rel_job);
    if (rel_job[id] != NULL)
    {
        if (rel_job[id]->job_state != STATE_PAUSE)
        {
            if (rel_job[id]->job_state != STATE_RUNNING)
            {
                pthread_mutex_unlock(&mut_rel_job);
                return -ECANCELED;
            }

            clock_gettime(CLOCK_REALTIME, &rel_job[id]->time_end);
            rel_job[id]->job_state = STATE_PAUSE;
        }
    }

    pthread_mutex_unlock(&mut_rel_job);

    syslog(LOG_INFO, "<INFO> [%s:%d]:exit rel_pausejob()", __FUNCTION__, __LINE__);
    return 0;
}

/**
 * return  == 0             成功，任务成功恢复（或本身就在运行）
 *         == -EINVAL       失败，参数非法
 *         == -ECANCLED     失败，指定任务无法恢复（已完成或并未暂停）
 */
int rel_resumejob(int id)
{
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter rel_resumejob()", __FUNCTION__, __LINE__);
    if (id < 0 || id > REL_JOBMAX)
    {
        return -EINVAL;
    }
    pthread_mutex_lock(&mut_rel_job);
    if (rel_job[id] != NULL)
    {
        if (rel_job[id]->job_state != STATE_RUNNING)
        {
            if (rel_job[id]->job_state != STATE_PAUSE)
            {
                pthread_mutex_unlock(&mut_rel_job);
                return -ECANCELED;
            }
            clock_gettime(CLOCK_REALTIME, &rel_job[id]->time_end);
            rel_job[id]->job_state = STATE_RUNNING;
        }
    }
    pthread_mutex_unlock(&mut_rel_job);

    syslog(LOG_INFO, "<INFO> [%s:%d]:exit rel_resumejob()", __FUNCTION__, __LINE__);
    return 0;
}

/**
 * return == 0              成功，回填任务状态
 *        == -EINVAL        失败，参数非法
 *
 */
int rel_statjob(int id, struct rel_stat_st *stat)
{
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter rel_statjob()", __FUNCTION__, __LINE__);
    if (id < 0 || id > REL_JOBMAX || stat == NULL)
    {
        return -EINVAL;
    }
    pthread_mutex_lock(&mut_rel_job);
    if (rel_job[id] != NULL)
    {
        if (stat != NULL)
        {
            stat->fd1 = rel_job[id]->fd1;
            stat->fd2 = rel_job[id]->fd2;
            stat->state = rel_job[id]->job_state;
            stat->time_start = rel_job[id]->time_start;
            stat->time_end = rel_job[id]->time_end;
            stat->count12 = rel_job[id]->fsm12.count;
            stat->count21 = rel_job[id]->fsm21.count;
        }
    }
    pthread_mutex_unlock(&mut_rel_job);

    syslog(LOG_INFO, "<INFO> [%s:%d]:exit rel_statjob()", __FUNCTION__, __LINE__);

    return 0;
}

/**
 * return == 0          成功，指定任务成功释放,并回填状态
 *        == -EINVAL    失败，参数非法
 *
 */
int rel_waitjob(int id, struct rel_stat_st *stat)
{
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter rel_waitjob()", __FUNCTION__, __LINE__);
    if (id < 0 || id > REL_JOBMAX)
    {
        return -EINVAL;
    }

    pthread_mutex_lock(&mut_rel_job);
    if (rel_job[id] != NULL)
    {
        if (stat != NULL)
        {
            stat->fd1 = rel_job[id]->fd1;
            stat->fd2 = rel_job[id]->fd2;
            stat->state = rel_job[id]->job_state;
            stat->time_start = rel_job[id]->time_start;
            stat->time_end = rel_job[id]->time_end;
            stat->count12 = rel_job[id]->fsm12.count;
            stat->count21 = rel_job[id]->fsm21.count;
        }
        fcntl(rel_job[id]->fd2, F_SETFL, rel_job[id]->fd2_save);
        fcntl(rel_job[id]->fd1, F_SETFL, rel_job[id]->fd1_save);
        free(rel_job[id]);
        rel_job[id] = NULL;
    }
    pthread_mutex_unlock(&mut_rel_job);

    syslog(LOG_INFO, "<INFO> [%s:%d]:exit rel_waitjob()", __FUNCTION__, __LINE__);
    return 0;
}

static int rel_waitjob_unlocked(int id, struct rel_stat_st *stat)
{
    syslog(LOG_INFO, "<INFO> [%s:%d]:enter rel_waitjob_unlocked()", __FUNCTION__, __LINE__);
    if (id < 0 || id > REL_JOBMAX)
    {
        return -EINVAL;
    }

    // pthread_mutex_lock(&mut_rel_job);
    if (rel_job[id] != NULL)
    {
        if (stat != NULL)
        {
            stat->fd1 = rel_job[id]->fd1;
            stat->fd2 = rel_job[id]->fd2;
            stat->state = rel_job[id]->job_state;
            stat->time_start = rel_job[id]->time_start;
            stat->time_end = rel_job[id]->time_end;
            stat->count12 = rel_job[id]->fsm12.count;
            stat->count21 = rel_job[id]->fsm21.count;
        }

        fcntl(rel_job[id]->fd2, F_SETFL, rel_job[id]->fd2_save);
        fcntl(rel_job[id]->fd1, F_SETFL, rel_job[id]->fd1_save);
        free(rel_job[id]);
        rel_job[id] = NULL;
    }
    //  pthread_mutex_unlock(&mut_rel_job);

    syslog(LOG_INFO, "<INFO> [%s:%d]:exit rel_waitjob_unlocked()", __FUNCTION__, __LINE__);
    return 0;
}