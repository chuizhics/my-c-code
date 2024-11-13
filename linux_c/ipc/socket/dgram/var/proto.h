#ifndef PROTO_H__
#define PROTO_H__

#define RCVPORT     1996   
// 512 - UDP包推荐长度
// 8 udp报头大小
// 8 数据结构中定长部分
#define NAMEMAX     (512-8-8)

#include <stdint.h>
 
struct msg_st
{
    uint32_t math;
    uint32_t Chinese;
    uint8_t name[0];
}__attribute__((packed));
 


#endif