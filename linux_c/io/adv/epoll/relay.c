#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/epoll.h>
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

    int epfd;
    struct epoll_event eev;

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

    // epfd = epoll_create1(0);
    epfd = epoll_create(1);
    if (epfd < 0)
    {
        perror("epoll_create()");
        exit(1);
    }

    eev.events = 0;
    eev.data.fd = fd1;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &eev);

    eev.events = 0;
    eev.data.fd = fd2;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &eev);

    while (fsm12.state != STATE_T || fsm21.state != STATE_T)
    {
        // 布置监视任务

        eev.data.fd = fd1;
        eev.events = 0;
        if (fsm12.state == STATE_R)
        {
            eev.events |= EPOLLIN;
        }
        if (fsm21.state == STATE_W)
        {
            eev.events |= EPOLLOUT;
        }
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &eev);


        eev.data.fd = fd2;
        eev.events = 0;
        if (fsm12.state == STATE_W)
        {
            eev.events |= EPOLLOUT;
        }

        if (fsm21.state == STATE_R)
        {
            eev.events |= EPOLLIN;
        }
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &eev);


        // 监视
        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
        {
            while (epoll_wait(epfd, &eev, 1, -1) < 0)
            {
                if (errno == EINTR)
                    continue;
                perror("epoll_wait()");
                exit(1);
            }
        }
        // 查看监视结果，有条件的推动状态机
        //1可读 2可写
        if (eev.data.fd == fd1 && eev.events & EPOLLIN \
            || eev.data.fd == fd2 && eev.events & EPOLLOUT \
             || fsm12.state > STATE_AUTO)
        {
            fsm_driver(&fsm12);
        }

        //2可读 1可写
        if (eev.data.fd == fd2 && eev.events & EPOLLIN \
            || eev.data.fd == fd1 && eev.events & EPOLLOUT \
             || fsm21.state > STATE_AUTO)
        {
            fsm_driver(&fsm21);
        }
    }

    fcntl(fd2, F_SETFL, fd2_save);
    fcntl(fd1, F_SETFL, fd1_save);

    close(epfd);
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
