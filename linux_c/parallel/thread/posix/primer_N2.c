#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define LEFT  30000000
#define RIGHT 30000200

#define N 3

struct thr_arg_st
{
    int n;
};

static void *thr_prime(void *arg)
{
    int i, j, mark, k;
    struct thr_arg_st *p = arg;
    i = p->n;
    for (k = LEFT + i; k < RIGHT; k += N)
    {
        mark = 1;
        for (j = 2; j <= k / 2; j++)
        {
            if (k % j == 0)
            {
                mark = 0;
                break;
            }
        }
        if (mark)
        {
              //printf("[Thread-%lu]%d is a primer.\n",pthread_self(),i);
            printf("%d is a primer.\n",k);
        }
    }

    pthread_exit(arg);
}

int main()
{
    int i, j, k;
    int err;
    pthread_t tid[N];
    struct thr_arg_st *ta_ptr = NULL;
    void *ptr;
    for (i = 0; i < N; i++)
    {
        ta_ptr = malloc(sizeof(*ta_ptr));
        if (ta_ptr == NULL)
        {
            perror("malloc()");
            exit(1);
        }
        ta_ptr->n = i;

        err = pthread_create(&tid[i], NULL, thr_prime, ta_ptr);
        if (err)
        {
            for (k = 0; k < i; k++)
            {
                pthread_cancel(tid[k]);
                pthread_join(tid[k], NULL);
            }
            fprintf(stderr, "pthread_create():%s\n", strerror(err));
            fflush(stderr);
            exit(1);
        }
    }

    for (i = 0; i < N; i++)
    {
        pthread_join(tid[i], &ptr);
        free(ptr);
    }

    exit(0);
}
