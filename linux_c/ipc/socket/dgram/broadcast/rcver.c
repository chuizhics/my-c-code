#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netinet/ip.h> /* superset of previous */

#include <arpa/inet.h>


#include "proto.h"


#define IPSTRSIZE 40

int main()
{
    int sd;
    struct sockaddr_in  laddr, raddr;
    struct msg_st rbuf;
    socklen_t raddr_len;
    char ipstr[IPSTRSIZE];

    int broadcast_flag = 1;

    sd =socket(AF_INET, SOCK_DGRAM, 0 /*IPPROTO_UDP*/);

    if(sd < 0)
    {
        perror("socket");
        exit(1);
    }

    if(setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&broadcast_flag,sizeof(broadcast_flag)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(RCVPORT);

    //point to number
    inet_pton(AF_INET,"0.0.0.0", &laddr.sin_addr.s_addr);


    if(bind(sd, (void *)&laddr,sizeof(laddr)) < 0)
    {
        perror("bind()");
        exit(1);
    }

    /**!!!!**/
    raddr_len = sizeof(raddr);


    while(1)
    {
        recvfrom(sd,&rbuf,sizeof(rbuf),0, (void *)&raddr, &raddr_len);
        inet_ntop(AF_INET,&raddr.sin_addr.s_addr,ipstr,IPSTRSIZE);

        printf("---MESSAGE FROM %s:%d\n",ipstr, ntohs(raddr.sin_port));

        printf("NAME = %s\n", rbuf.name);

        printf("MATH = %d\n", ntohl(rbuf.math));

        printf("CHINESE = %d\n", ntohl(rbuf.Chinese));
    }

    close(sd);


    exit(0);
}