#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <time.h>
#include <errno.h>

#include "proto.h"

#define IPSTRSIZE 40

#define BUFSIZE 1024

static void server_job(int sd)
{
    char buf[BUFSIZE];
    int len = 0;
    ssize_t nsend;

    len = sprintf(buf, FMT_STAMP, (long long)time(NULL));

    nsend = send(sd, buf, len, 0);
    if (nsend < 0)
    {
        perror("send()");
        exit(1);
    }
}

int main()
{
    int sd, newsd;
    socklen_t raddr_len;
    struct sockaddr_in laddr, raddr;
    char ipstr[IPSTRSIZE];
    int val;
    pid_t pid;

    sd = socket(AF_INET, SOCK_STREAM, 0 /*IPPTOTO_TCP,IPPROTO_SCTP*/);
    if (sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
    {
        perror("setsockopt()");
        close(sd);
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    // point to number
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);

    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
    {
        close(sd);
        perror("bind()");
        exit(1);
    }

    // 200 -》全连接队列大小
    if (listen(sd, 200) < 0)
    {
        close(sd);
        perror("listen()");
        exit(1);
    }

    raddr_len = sizeof(raddr);

    while (1)
    {
        newsd = accept(sd, (void *)&raddr, &raddr_len);
        if (newsd < 0)
        {
            if (errno == EAGAIN || errno == EINTR)
            {
                continue;
            }
            close(sd);
            perror("accept()");
            exit(1);
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(1);
        }
        if (pid == 0)
        {
            /**!!!!!***/
            close(sd);  // 子进程用不到，需要关闭
            inet_ntop(AF_INET, (void *)&raddr.sin_addr, ipstr, IPSTRSIZE);
            printf("sd = %d, newsd = %d\n", sd, newsd);
            printf("Clinet:%s:%d\n", ipstr, ntohs(raddr.sin_port));
            server_job(newsd);
            close(newsd);
            exit(0);
        }
        close(newsd);  // 父进程用不到，需要关闭
    }

    close(sd);
    exit(0);
}