#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "proto.h"

static int msg_id;

static void print_msg(const struct msg_st *msg)
{
    if(msg != NULL)
    {
        printf("\n");
        printf("name:%s\n",msg->name);
        printf("math:%d\n",msg->math);
        printf("chinese:%d\n",msg->chinese);
    }
}


int main()
{
    key_t key;
    
    key = ftok(KEYPATH, KEYPROJ);
    struct msg_st msg;
    
    if (key < 0)
    {
        perror("ftok()");
        exit(1);
    }

    msg_id = msgget(key, IPC_CREAT | 0600);
    if(msg_id < 0)
    {
        perror("msgget()");
        exit(1);
    }


    //参数 msgsz 指定 msgp 参数指向的结构的成员 mtext 的最大大小（以字节为单位）。
    // 如果 msgtyp 为 0，则读取队列中的第一条消息。
    while(1)
    {
        if(msgrcv(msg_id, &msg, (sizeof(struct msg_st) - sizeof(long)), 0, 0) < 0)
        {
            perror("msgrcv()");
            exit(1);
        }
        print_msg(&msg);
    }
    
    msgctl(msg_id,IPC_RMID, NULL);


    exit(0);
}