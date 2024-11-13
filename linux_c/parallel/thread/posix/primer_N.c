#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define LEFT  30000000
#define RIGHT 30000200

#define THRNUM (RIGHT - LEFT)

#define N 3

struct thr_arg_st
{
    int num[THRNUM];
    int num_length;
};

static void *thr_prime(void *arg)
{
    int i, j, mark, k;
    struct thr_arg_st *p = arg;
    for (k = 0; k < p->num_length; k++)
    {
        i = p->num[k];
        //printf("[Thread-%lu]%d is testing......\n",pthread_self(),i);
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
              //printf("[Thread-%lu]%d is a primer.\n",pthread_self(),i);
            printf("%d is a primer.\n",i);
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
        ta_ptr->num_length = 0;
        for (j = LEFT + i; j < RIGHT; j += N)
        {
            ta_ptr->num[ta_ptr->num_length++] = j;
        }

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
