#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>
//  测试出来 TID_MAX 约等于  9389    原因进程号耗尽

#define TID_MAX  (1024 * 32)

static void *func(void *arg)
{
    // int i = 5;
    // printf("%ld\n", &i);

    while(1)
    {
        pause();
    }

    pthread_exit(NULL);
}


int main()
{
    int i, err, j;
    pthread_t tid[TID_MAX];

    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_attr_setstacksize(&attr, 1024L * 1024L);


    for(i = 0; ;i++)
    {
        err = pthread_create(&tid[i], &attr,func, NULL);
        if(err)
        {
            fprintf(stderr,"pthread_create(): %s\n", strerror(err));
            break;
        }
    }
    j = i;
    while (--j)
    {
        pthread_cancel(tid[j]);
        pthread_join(tid[j], NULL);
    }
    
    pthread_attr_destroy(&attr);

    printf("i = %d\n", i);
   
    

    exit(0);
}