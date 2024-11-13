#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define CHARACTERPS 10

#define BUFSIZE CHARACTERPS

static volatile sig_atomic_t loop = 0;

static void slowcat_alarm_handler(int sig)
{
    // alarm(1);
    loop = 1;
    // printf("sig = %d\n",sig);  sig = 14
}

int main(int argc, char *argv[])
{
    int dest_fd = STDOUT_FILENO, src_fd;
    int pos = 0;
    char buf[BUFSIZE];
    ssize_t nread = 0;
    ssize_t nwrite = 0;
    struct itimerval it_val;

    if (argc < 2)
    {
        fprintf(stderr, "Usage:%s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGALRM, slowcat_alarm_handler);

    it_val.it_interval.tv_sec = 1;   // 周期值
    it_val.it_interval.tv_usec = 0;
    it_val.it_value.tv_sec = 1;    // 初始值
    it_val.it_value.tv_usec = 0;

    // alarm(1);
    if(setitimer(ITIMER_REAL, &it_val, NULL) < 0)
    {
        perror("setitimer()");
        exit(1);
    }

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
        while (!loop)
        {
            /*不忙等**/
            pause();
        }
        loop = 0;

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
        // sleep(1);
    }

    close(src_fd);

    exit(EXIT_SUCCESS);
}