#ifndef PROTO_H__
#define PROTO_H__

#define RCVPORT   1996   

#define NAMESIZE  11

#include <stdint.h>

struct msg_st
{
    
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t Chinese;
    //char name[NAMESIZE];
    //int math;
    //int Chinese;
}__attribute__((packed));





#endif