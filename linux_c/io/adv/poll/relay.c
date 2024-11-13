#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"

#define BUFSIZE 1024

enum
{
    STATE_R = 1,
    STATE_W,
    STATE_AUTO,
    STATE_Ex,
    STATE_T
};

struct rel_fsm_st
{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int nread;
    int pos;
    char *errstr;
};

static void fsm_driver(struct rel_fsm_st *fsm)
{
    int nwrite;

    if (fsm != NULL)
    {
        switch (fsm->state)
        {
        case STATE_R:
            fsm->nread = read(fsm->sfd, fsm->buf, BUFSIZE);
            if (fsm->nread == 0)
            {
                fsm->state = STATE_T;
            }
            else if (fsm->nread < 0)
            {
                if (errno == EAGAIN)
                {
                    fsm->state = STATE_R;
                }
                else
                {
                    fsm->errstr = "read()";
                    fsm->state = STATE_Ex;
                }
            }
            else
            {
                fsm->pos = 0;
                fsm->state = STATE_W;
            }

            break;
        case STATE_W:
            nwrite = write(fsm->dfd, fsm->buf + fsm->pos, fsm->nread);
            if (nwrite < 0)
            {
                if (errno == EAGAIN)
                {
                    fsm->state = STATE_W;
                }
                else
                {
                    fsm->errstr = "write()";
                    fsm->state = STATE_Ex;
                }
            }
            else
            {
                fsm->pos += nwrite;
                fsm->nread -= nwrite;
                if (fsm->nread == 0)
                {
                    fsm->state = STATE_R;
                }
                else
                {
                    fsm->state = STATE_W;
                }
            }

            break;

        case STATE_Ex:
            perror(fsm->errstr);
            fsm->state = STATE_T;
            break;
        case STATE_T:
            /*do sth*/
            break;
        default:
            abort();
            break;
        }
    }
}

static void relay(int fd1, int fd2)
{
    int fd1_save, fd2_save;

    struct rel_fsm_st fsm12, fsm21;

    struct pollfd pfd[2];

    fd1_save = fcntl(fd1, F_GETFL);
    if (fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK) == -1)
    {
        perror("fcntl()");
        exit(1);
    }

    fd2_save = fcntl(fd2, F_GETFL);
    if (fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK) == -1)
    {
        fcntl(fd1, F_SETFL, fd1_save);
        perror("fcntl()");
        exit(1);
    }

    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    pfd[0].fd = fd1;
    pfd[1].fd = fd2;

    while (fsm12.state != STATE_T || fsm21.state != STATE_T)
    {
        // 布置监视任务
        // 位图清零
        pfd[0].events = 0;
        if (fsm12.state == STATE_R)
        {
            pfd[0].events |= POLLIN;
        }
        if (fsm21.state == STATE_W)
        {
            pfd[0].events |= POLLOUT;
        }

        pfd[1].events = 0;
        if (fsm12.state == STATE_W)
        {
            pfd[1].events |= POLLOUT;
        }

        if (fsm21.state == STATE_R)
        {
            pfd[1].events |= POLLIN;
        }

        // 监视
        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
        {
            while(poll(pfd, 2, -1) < 0)
            {
                if (errno == EINTR)
                    continue;
                perror("poll()");
                exit(1);
            }
        }
        // 查看监视结果，有条件的推动状态机

        if (pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT || fsm12.state > STATE_AUTO)
        {
            fsm_driver(&fsm12);
        }
        if (pfd[1].revents & POLLIN || pfd[0].revents & POLLOUT || fsm21.state > STATE_AUTO)
        {
            fsm_driver(&fsm21);
        }
    }

    fcntl(fd2, F_SETFL, fd2_save);
    fcntl(fd1, F_SETFL, fd1_save);
}

int main()
{
    int fd1, fd2;

    fd1 = open(TTY1, O_RDWR);
    if (fd1 < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd1, "TTY1\n", 5);

    fd2 = open(TTY2, O_RDWR | O_NONBLOCK);
    if (fd2 < 0)
    {
        perror("open()");
        close(fd1);
        exit(1);
    }

    write(fd2, "TTY2\n", 5);

    relay(fd1, fd2);

    close(fd2);
    close(fd1);

    exit(0);
}
