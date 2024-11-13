#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <string.h>

#include "proto.h"

int main(int argc, char *argv[])
{
    int sd;

    struct msg_st *sbufp;

    struct sockaddr_in raddr;

    socklen_t raddr_len;

    ssize_t nsend;

    size_t name_len, sbuf_size;

    if (argc != 3)
    {
        fprintf(stderr, "Usage:%s <ip> <name>\n", argv[0]);
        exit(1);
    }
    name_len = strlen(argv[2]);
    if (name_len + 1 > NAMEMAX)
    {
        fprintf(stderr, "name is too long!\n");
        exit(1);
    }

    sbuf_size = sizeof(struct msg_st) + name_len + 1;
    sbufp = malloc(sbuf_size);
    if (sbufp == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket");
        free(sbufp);
        exit(1);
    }
    // bind();

    // sprintf(sbuf.name,"Alan");
    // memcpy(sbufp->name,argv[2],name_len + 1);
    strcpy(sbufp->name, argv[2]);
    sbufp->math = htonl(75u);
    sbufp->Chinese = htonl(77u);
 
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(RCVPORT);
    // raddr.sin_addr.s_addr = atoi(argv[1]);
    inet_pton(AF_INET, argv[1], &raddr.sin_addr.s_addr);
    raddr_len = sizeof(raddr);

    if ((nsend = sendto(sd, sbufp, sbuf_size, 0, (void *)&raddr, raddr_len)) < 0)
    {
        perror("sendto()");
        exit(1);
    }

    close(sd);

    //printf("sbuf_size = %ld, sizeof(sbuf_size) = %ld nsend = %ld\n",sbuf_size,sizeof(sbuf_size), nsend);
    printf("NAME = %s\n", sbufp->name);

    printf("MATH = %d\n", ntohl(sbufp->math));

    printf("CHINESE = %d\n", ntohl(sbufp->Chinese));
    puts("OK!");
    free(sbufp);
    exit(0);
}