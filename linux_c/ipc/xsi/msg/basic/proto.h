#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH     "/etc/services"

// 定义为‘g’ 表示使用g的ascii码整形值
#define KEYPROJ     'g'

#define NAMESIZE    32


struct msg_st
{
    long mtype;   /* message type, must be > 0 */
    char name[NAMESIZE];
    int math;
    int chinese;
};






#endif