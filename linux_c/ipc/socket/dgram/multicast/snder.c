#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <unistd.h>
#include <string.h>

#include "proto.h"
 
int main(int argc, char *argv[])
{
    int sd;

    struct msg_st sbuf;

    struct sockaddr_in raddr;

    socklen_t raddr_len;

    ssize_t nsendto;

    // int broadcast_flag = 1;

    struct ip_mreqn mreq;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket");
        exit(1);
    }

    inet_pton(AF_INET,MGROUP,&mreq.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("ens33");

    if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }


    memset(&sbuf,'\0',sizeof(sbuf));
    // sprintf(sbuf.name,"Alan");
    memcpy(sbuf.name, "Alan", 5);
    sbuf.math = htonl(rand()%100);
    sbuf.Chinese = htonl(rand()%100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(RCVPORT);
    // raddr.sin_addr.s_addr = atoi(argv[1]);
    inet_pton(AF_INET,MGROUP, &raddr.sin_addr.s_addr);
    raddr_len = sizeof(raddr);

    if ((nsendto = sendto(sd, &sbuf, sizeof(sbuf), 0, (void *)&raddr, raddr_len)) < 0)
    {
        perror("sendto()");
        exit(1);
    }

    puts("OK");
    close(sd);

    exit(0);
}