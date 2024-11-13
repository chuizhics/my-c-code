#ifndef RELAYER_H__
#define RELAYER_H__

#include <sys/types.h>

#include <sys/time.h>

#define REL_JOBMAX      10000


enum
{
    // STATE_WAITING = 1, /*等待运行*/
    STATE_RUNNING = 1, /*正在运行*/
    STATE_PAUSE,   /*暂停*/
    STATE_OVER,    /*执行完成*/
    STATE_CANCELED   /*已取消*/
};



struct rel_stat_st
{
    int state;
    int fd1;
    int fd2;
    int64_t count12;
    int64_t count21;
    struct timespec time_start;
    struct timespec time_end;
};


/**
 * return   >= 0            成功，返回当前任务ID
 *          == -EINVAL      失败，参数非法
 *          == -ENOSPC      失败，任务数组满
 *          == -ENOMEM      失败，内存分配有误
*/
int rel_addjob(int fd1, int fd2);


/**
 * return  == 0             成功，指定任务成功取消（包括重复取消）
 *         == -EINVAL       失败，参数非法
 *         //== -EOWNERDEAD   失败，指定任务已完成
 *         // == -ECANCELED    失败，指定任务重复取消
 *         == -ECANCELD     失败，指定任务无法取消（已完成）
*/
int rel_canceljob(int id);


/**
 * return  == 0             成功，任务成功暂停
 *         == -EINVAL       失败，参数非法
 *         == -ECANCLED     失败，指定任务无法暂停（已完成或者是CANCELED）
*/
int rel_pausejob(int id);


/**
 * return  == 0             成功，任务成功恢复（或本身就在运行）
 *         == -EINVAL       失败，参数非法
 *         == -ECANCLED     失败，指定任务无法恢复（已完成或并未暂停）
*/
int rel_resumejob(int id);

/**
 * return == 0              成功，回填任务状态
 *        == -EINVAL        失败，参数非法
 * 
*/
int rel_statjob(int id, struct rel_stat_st *);

/**
 * return == 0          成功，指定任务成功释放,并回填状态
 *        == -EINVAL    失败，参数非法
 * 
*/
int rel_waitjob(int id, struct rel_stat_st *);


#endif