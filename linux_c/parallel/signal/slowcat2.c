#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define CHARACTERPS 10

#define BUFSIZE CHARACTERPS

#define BURST 100   //token 上限

/*sig_atomic_t 保证修饰的值的赋值取值操作原子化*/
static volatile sig_atomic_t token = 0;

static void slowcat_alarm_handler(int sig)
{
    alarm(1);
    token++;
    if(token > BURST)
        token = BURST;
    // printf("sig = %d\n",sig);  sig = 14
}

/**
 * 
 * 令牌桶实现 token
 * 
 * 
*/
int main(int argc, char *argv[])
{
    int src_fd;
    int dest_fd = STDOUT_FILENO;
    int pos = 0;
    char buf[BUFSIZE];
    ssize_t nread = 0;
    ssize_t nwrite = 0;
    if (argc < 2)
    {
        fprintf(stderr, "Usage:%s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    signal(SIGALRM, slowcat_alarm_handler);
    alarm(1);

    do
    {
        src_fd = open(argv[1], O_RDONLY);

        if (src_fd < 0)
        {
            if (errno != EINTR)
            {
                perror("open()");
                exit(EXIT_FAILURE);
            }
        }

    } while (src_fd < 0);

    while (1)
    {
        while (token <= 0)
        {
            /*不忙等**/
            pause();
        }
        token--;

        while ((nread = read(src_fd, buf, BUFSIZE)) < 0)
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
                exit(EXIT_FAILURE);
            }
            nread -= nwrite;
            pos += nwrite;
        }

    }

    close(src_fd);

    exit(EXIT_SUCCESS);
}