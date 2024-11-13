#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "mytbf.h"

#define CPS 10

#define BUFSIZE 1024

#define BURST 100 

    
static int src_fd = -1;
static mytbf_t *tbf = NULL;

static void safe_exit(int sig)
{
    if(src_fd >= 0)
    {
        close(src_fd);
    }
    if(tbf != NULL)
    {
        mytbf_destroy(tbf);
    }
    if(sig)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}


static void register_signal(void)
{
    struct sigaction sact;

    // sact.sa_flags = SA_SIGINFO;
    sact.sa_flags = 0;
    sact.sa_handler = safe_exit;
    
    sigemptyset(&sact.sa_mask);
    // sigaddset()   将哪些信号block住，避免重入现象
    sigaddset(&sact.sa_mask, SIGINT);
    sigaddset(&sact.sa_mask, SIGQUIT);
    sigaddset(&sact.sa_mask, SIGTERM);

    sigaction(SIGINT, &sact, NULL);
    sigaction(SIGQUIT, &sact, NULL);
    sigaction(SIGTERM, &sact, NULL);

}



int main(int argc, char *argv[])
{
    int dest_fd = STDOUT_FILENO;
    int pos = 0;
    char buf[BUFSIZE];
    ssize_t nread = 0;
    ssize_t nwrite = 0;

    int token = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage:%s <filename>\n", argv[0]);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    register_signal();

    tbf = mytbf_init(CPS,BURST);
    if(tbf == NULL)
    {
        fprintf(stderr,"mytbf_init() failed!\n");
        fflush(stderr);
        safe_exit(-1);
    }

    do
    {
        src_fd = open(argv[1], O_RDONLY);

        if (src_fd < 0)
        {
            if (errno != EINTR)
            {
                perror("open()");
                safe_exit(-1);
            }
        }

    } while (src_fd < 0);

    while (1)
    {
        token =  mytbf_fetchtoken(tbf,BUFSIZE);
       // printf("[%s:%d]:token = %d\n", __FUNCTION__,__LINE__,token);
        if(token < 0)
        {
            fprintf(stderr,"mytbf_fetchtoken():%s\n", strerror(-token));
            close(src_fd);
            safe_exit(-1);
        }

        while ((nread = read(src_fd, buf, token)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("read()");
                break;
            }
        }

        if (nread == 0)
            break;


        if(token - nread > 0)
        {
            mytbf_returntoken(tbf,token - nread);
        }

        pos = 0;
        while (nread > 0)
        {
            while ((nwrite = write(dest_fd, buf + pos, nread)) < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                close(src_fd);
                perror("write()");
                safe_exit(-1);
            }
            nread -= nwrite;
            pos += nwrite;
        }

    }
    safe_exit(0);
}