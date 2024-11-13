#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>

#define LEFT 30000000
#define RIGHT 30000200

#define THRNUM (RIGHT - LEFT)

//static volatile int count = 0;

#define N  3


struct thr_arg_st
{
    int n;
};

static void cleanup_func(void *arg)
{
    // printf("[%s:%d]cleanup_func()..count = %d\n", __FUNCTION__, __LINE__, count++);
    pthread_join(*((pthread_t *)arg), NULL);
}

static void *thr_prime(void *arg)
{
    int i, j, mark;

    //i = *((int *)arg);
    // i = (int)arg;
    i = ((struct thr_arg_st *)arg)->n;
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
        printf("%d is a primer.\n", i);
    }

    pthread_exit(arg);
}

int main()
{
    int i, j;
    int err;
    int cancel_err, join_err;
    pthread_t tid[THRNUM];
    struct thr_arg_st *ta_ptr = NULL;
    void *ptr;

    for (i = LEFT; i < RIGHT; i++)
    {
        ta_ptr = malloc(sizeof(*ta_ptr));
        if(ta_ptr == NULL)
        {
            perror("malloc()");
            exit(1);
        }
        ta_ptr->n = i;
        //err = pthread_create(&tid[i - LEFT], NULL, thr_prime, (void *)i);

        err = pthread_create(&tid[i - LEFT], NULL, thr_prime, ta_ptr);
        // pthread_cleanup_push(cleanup_func, &tid[i - LEFT]);
        // pthread_cleanup_pop(1);
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

    for (i = 0; i < THRNUM; i++)
    {
        pthread_join(tid[i], &ptr);
        free(ptr);
    }

    exit(0);
}
