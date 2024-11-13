#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "mypipe.h"

struct mypipe_st
{
    int head;
    int tail;
    char data[PIPESIZE];
    int datasize;
    int count_reader;
    int count_writer;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};

mypipe_t *mypipe_init(void)
{
    struct mypipe_st *me = NULL;

    me = malloc(sizeof(*me));
    if (me == NULL)
    {
        return NULL;
    }
    me->head = 0;
    me->tail = 0;
    me->datasize = 0;
    me->count_reader = 0;
    me->count_writer = 0;
    pthread_mutex_init(&me->mut, NULL);
    pthread_cond_init(&me->cond, NULL);

    return me;
}

int mypipe_register(mypipe_t *pipe, int opmap)
{
    struct mypipe_st *me = pipe;

    pthread_mutex_lock(&me->mut);

    if(opmap & MYPIPE_READ)
    {
        me->count_reader++;
    }
    if(opmap & MYPIPE_WRITE)
    {
        me->count_writer++;
    }

    while(me->count_reader <= 0 || me->count_writer <= 0)
    {
        pthread_cond_wait(&me->cond,&me->mut);
    }

    pthread_cond_broadcast(&me->cond);

    pthread_mutex_unlock(&me->mut);
    return 0;
}

int mypipe_unregister(mypipe_t *pipe, int opmap)
{
    struct mypipe_st *me = pipe;
    pthread_mutex_lock(&me->mut);

    if(opmap & MYPIPE_READ)
    {
        me->count_reader--;
    }
    if(opmap & MYPIPE_WRITE)
    {
        me->count_writer--;
    }

    pthread_cond_broadcast(&me->cond);
    
    pthread_mutex_unlock(&me->mut);
    return 0;
}


static int nexthead(int head)
{
    if(head + 1 == PIPESIZE)
    {
        return 0;
    }
    return head + 1;
}

static int mypipe_readbyte_unlocked(struct mypipe_st *me, char *datap)
{
    if(me->datasize <= 0)
    {
        return -1;
    }

    *datap = me->data[me->head];
    me->head = nexthead(me->head);
    me->datasize--;
    return 0;
}

int mypipe_read(mypipe_t *pipe, void *buf, size_t count)
{
    size_t i;
    struct mypipe_st *me = pipe;

    pthread_mutex_lock(&me->mut);

    while (me->datasize <= 0 && me->count_writer > 0)
    {
        pthread_cond_wait(&me->cond, &me->mut);
    }

    if(me->datasize <= 0 && me->count_writer <= 0)
    {
        pthread_mutex_unlock(&me->mut);
        return 0;
    }

    for (i = 0; i < count; i++)
    {
        if(mypipe_readbyte_unlocked(me, buf + i) != 0)
        {
            break;
        }
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return i;
}

int mypipe_write(mypipe_t *pipe, const void *buf, size_t count)
{

    return 0;
}

int mypipe_destroy(mypipe_t *pipe)
{
    struct mypipe_st *me = pipe;
    if (me != NULL)
    {
        pthread_mutex_destroy(&me->mut);
        pthread_cond_destroy(&me->cond);
        free(me);
    }
    return 0;
}