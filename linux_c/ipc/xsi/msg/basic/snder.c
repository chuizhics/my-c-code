#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <errno.h>
#include "proto.h"

int main()
{
    key_t key;
    int msg_id;
    int i;
    struct msg_st *msg = NULL;

    key = ftok(KEYPATH, KEYPROJ);

    if (key < 0)
    {
        perror("ftok()");
        exit(1);
    }

    msg_id = msgget(key, 0);
    if (msg_id < 0)
    {
        perror("msgget()");
        exit(1);
    }

    for (i = 0; i < 5; i++)
    {
        msg = malloc(sizeof(*msg));
        if (msg == NULL)
        {
            perror("malloc()");
            exit(1);
        }
        msg->mtype = 1;
        snprintf(msg->name, NAMESIZE, "stu%d", i);
        msg->math = rand() % 100;
        msg->chinese = rand() % 100;

        while (msgsnd(msg_id, msg, sizeof(struct msg_st) - sizeof(long), 0) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("msgsnd");
            exit(1);
        }
        free(msg);
    }

    exit(0);
}