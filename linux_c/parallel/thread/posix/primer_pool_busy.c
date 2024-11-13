#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>

#define LEFT 30000000
#define RIGHT 30000200

#define THRNUM 4

static int num = 0;

static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

static void *thr_prime(void *arg)
{
    int i, j, mark;

    while (1)
    {
        pthread_mutex_lock(&mut_num);
        if (num == 0)
        {
            pthread_mutex_unlock(&mut_num);
            sched_yield();
        }
        else if (num == -1)
        {
            pthread_mutex_unlock(&mut_num);
            break;
        }
        else
        {
            i = num;
            num = 0;
            pthread_mutex_unlock(&mut_num);
            mark = 1;
            for (j = 2; j <= i / 2; j++)
            {
                if (i % j == 0)
                {
                    mark = 0;
                    break;
                }
            }
            if (mark)
            {
                printf("[Thread-%d] %d is a primer.\n", (int)arg,i);
            }
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int i, j;
    int err;
    int cancel_err, join_err;
    pthread_t tid[THRNUM];
    struct thr_arg_st *ta_ptr = NULL;
    void *ptr;

    for (i = 0; i < THRNUM; i++)
    {
        err = pthread_create(&tid[i], NULL, thr_prime, (void *)i);
        if (err)
        {
            for (j = 0; j < i - LEFT; j++)
            {
                if ((cancel_err = pthread_cancel(tid[j])) != 0)
                {
                    fprintf(stderr, "pthread_cancel():%s\n", strerror(cancel_err));
                }
                if ((join_err = pthread_join(tid[j], NULL)) != 0)
                {
                    fprintf(stderr, "pthread_join():%s\n", strerror(join_err));
                }
            }
            fprintf(stderr, "pthread_create():%s\n", strerror(err));
            fflush(stderr);
            exit(1);
        }
    }

    for (i = LEFT; i < RIGHT; i++)
    {
        pthread_mutex_lock(&mut_num);

        while (num != 0)
        {
            pthread_mutex_unlock(&mut_num);

            sched_yield();

            pthread_mutex_lock(&mut_num);
        }
        num = i;

        pthread_mutex_unlock(&mut_num);
    }

    while (1)
    {
        pthread_mutex_lock(&mut_num);
        if (num == 0)
        {
            num = -1;
            pthread_mutex_unlock(&mut_num);
            break;
        }
        pthread_mutex_unlock(&mut_num);
        sched_yield();
    }

    for (i = 0; i < THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mut_num);

    exit(0);
}
