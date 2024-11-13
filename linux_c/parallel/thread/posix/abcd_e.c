#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#define THRNUM 4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static pthread_t tid[THRNUM];

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
    int n = (int)arg;
    int ch = 'a' + n;

    while (1)
    {
        pthread_mutex_lock(&mut_num);
        
        while (num != n && num != -1)
        {
            pthread_cond_wait(&cond_num, &mut_num);
        }
        if(num == -1)
        {
            pthread_mutex_unlock(&mut_num);
            break;
        }

        //write(1, &ch, 1);
        printf("%c",ch);
        num = next(num);
        pthread_cond_broadcast(&cond_num);
        pthread_mutex_unlock(&mut_num);
    }

    pthread_exit(NULL);
}

static void alrm_handler(int sig)
{
    printf("\nrecieved signal %d\n", sig);
    int i;
    int cancel_ret, join_ret;

    pthread_mutex_lock(&mut_num);
    
    num = -1;   // 发送退出信号
    pthread_cond_broadcast(&cond_num);

    pthread_mutex_unlock(&mut_num);

    for (i = 0; i < THRNUM; i++)
    {
        // cancel_ret =  pthread_cancel(tid[i]);
        // if(cancel_ret)
        // {
        //     fprintf(stderr, "\n [%d] cancel failed:%s.\n", i,strerror(cancel_ret));
        // }
        // printf("\n[%d] cancel finished.\n", i);
        pthread_join(tid[i], NULL);
        // if(join_ret)
        // {
        //     fprintf(stderr, "\n [%d] join failed:%s.\n", i,strerror(join_ret));
        // }
        // printf("\n[%d] join  finished. \n", i);
    }

   

    // pthread_mutex_lock(&mut_num);
    // pthread_cond_broadcast(&cond_num);
    // pthread_mutex_unlock(&mut_num);

    // if (0 != cond_num.__data.__wrefs)
    // {
    //     cond_num.__data.__wrefs = 0;
    // }

    pthread_cond_destroy(&cond_num);

    pthread_mutex_destroy(&mut_num);

    

    exit(0);
}

int main()
{
    int i, err;

    for (i = 0; i < THRNUM; i++)
    {
        err = pthread_create(tid + i, NULL, thr_func, (void *)i);
        if (err)
        {
            fprintf(stderr, "pthread_create():%s\n", strerror(err));
            fflush(stderr);
            exit(1);
        }
    }

    signal(SIGALRM, alrm_handler);

    alarm(3);

    while (1)
    {
        pause();
    }

    exit(0);
}