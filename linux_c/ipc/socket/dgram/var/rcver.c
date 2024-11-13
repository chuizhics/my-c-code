#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
 
#include <string.h>

#include "proto.h"

#define IPSTRSIZE 40

int main()
{
    int sd;
    size_t rbuf_size;
    struct sockaddr_in laddr, raddr;
    struct msg_st *rbufp;
    socklen_t raddr_len;
    char ipstr[IPSTRSIZE];

    ssize_t nrecv;

    rbuf_size = sizeof(struct msg_st) + NAMEMAX;

    rbufp = malloc(rbuf_size);
    if (rbufp == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0 /*IPPROTO_UDP*/);

    if (sd < 0)
    {
        perror("socket");
        free(rbufp);
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(RCVPORT);

    // point to number
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);

    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
    {
        perror("bind()");
        free(rbufp);
        exit(1);
    }

    /**!!!!**/
    raddr_len = sizeof(raddr);

    //printf("rbuf_size = %ld\n", rbuf_size);
    while (1)
    {
        nrecv = recvfrom(sd, rbufp, rbuf_size, 0, (void *)&raddr, &raddr_len);
        if(nrecv < 0)
        {
            perror("recvfrom");
            free(rbufp);
            exit(1);
        }
        inet_ntop(AF_INET, &raddr.sin_addr.s_addr, ipstr, IPSTRSIZE);

        printf("---MESSAGE FROM %s:%d\n", ipstr, ntohs(raddr.sin_port));

        //printf("nrecv = %ld\n",nrecv);
        printf("NAME_LEN = %ld NAME = %s\n", strlen(rbufp->name), rbufp->name);

        printf("MATH = %d\n", ntohl(rbufp->math));

        printf("CHINESE = %d\n", ntohl(rbufp->Chinese));

        fflush(NULL);
    }

    close(sd);
    free(rbufp);

    exit(0);
}