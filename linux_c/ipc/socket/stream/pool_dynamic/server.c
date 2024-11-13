#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <time.h>
#include <errno.h>

#include <sys/wait.h>

#include "proto.h"

#define MINSPARESERVER   5
#define MAXSPARESERVER  10
#define CLIENT          20



int main()
{

    socket();

    setsockopt();

    bind();

    listen();

    

    exit(0);
}