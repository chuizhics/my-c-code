#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/select.h>
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

static int max(int a, int b)
{
    return a > b ? a : b;
}

static void relay(int fd1, int fd2)
{
    int fd1_save, fd2_save;

    struct rel_fsm_st fsm12, fsm21;

    fd_set rset, wset;

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

    while (fsm12.state != STATE_T || fsm21.state != STATE_T)
    {
        // 布置监视任务
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        if (fsm12.state == STATE_R)
        {
            FD_SET(fsm12.sfd, &rset);
        }
        if (fsm12.state == STATE_W)
        {
            FD_SET(fsm12.dfd, &wset);
        }

        if (fsm21.state == STATE_R)
        {
            FD_SET(fsm21.sfd, &rset);
        }

        if (fsm21.state == STATE_W)
        {
            FD_SET(fsm21.dfd, &wset);
        }

        // 监视
        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
        {
            if (select(max(fd1, fd2) + 1, &rset, &wset, NULL, NULL) < 0)
            {
                if (errno == EINTR)
                    continue;
                perror("select()");
                exit(1);
            }
        }
        // 查看监视结果，有条件的推动状态机


        if (FD_ISSET(fd1, &rset) || FD_ISSET(fd2, &wset) || fsm12.state > STATE_AUTO)
        {
            fsm_driver(&fsm12);
        }
        if (FD_ISSET(fd2, &rset) || FD_ISSET(fd1, &wset) || fsm21.state > STATE_AUTO)
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
