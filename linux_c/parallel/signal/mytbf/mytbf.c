#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <signal.h>
#include <unistd.h>


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

// equals static __sighandler_t alrm_handler_save;
static void (*alrm_handler_save)(int);



static void alrm_handler(int sig)
{
    int i = 0;


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

    alarm(1);
}


static void module_unload(void)
{
    int i;
    signal(SIGALRM, alrm_handler_save);
    alarm(0);

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
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);

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
    struct mytbf_st *tbf = ptr;
    int ret = 0;

    if (size <= 0)
    {
        return -EINVAL;
    }

    while (tbf->token <= 0)
    {
        pause();
    }

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
    struct mytbf_st *tbf = ptr;
    job[tbf->pos] = NULL;
    free(tbf);

    return 0;
}
