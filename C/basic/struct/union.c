#include <stdio.h>
#include <stdlib.h>

#define NAMESIZE 32

union simple_un
{
    int i;
    float j;
    struct 
    {
        int id;
        char name[NAMESIZE];
    }stu ;
};


union num_un
{
    struct 
    {
        __uint16_t i;  //low bit
        __uint16_t j;  // high bit
    };
    __uint32_t x;
};



int main()
{
    __uint32_t num = 0x11223344;

   // union num_un num_u = {num};
    union num_un num_u;
    num_u.x = num;

    printf("num_u.x = %x\n", num_u.x);
    printf("num_u.i = %x\n", num_u.i);
    printf("num_u.j = %x\n", num_u.j);


    exit(0);
}