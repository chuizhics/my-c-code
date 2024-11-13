#include <stdio.h>
#include <stdlib.h>

#include <string.h>

struct st1
{
    int i;
    float f;
    char ch;
};


struct st2
{
    int i;
    char ch;
    float f;
};

struct st3
{
    int i;
    char ch2;
    float f;
    char ch;
};

struct st4
{
    char ch;
    int i;
    float f;
};

struct st5
{
    char ch;
    int i;
    char name[32];
    float f;
};

struct st6
{
    char ch;
    int i;
    char name[30];
    float f;
};

struct st7
{
    int i;
    float f;
    char name[0];
};

struct st8
{
    int i;
    float f;
    char name[1];
};



struct test
{
    char x1;
    short x2;
    float x3;
    char x4;
};

struct A
{
    int a;
    char b;
    short c;
};
struct B
{
    char b;
    int a;
    short c;
};



/**
 * 
 *  psize = 16, sizeof(*p) = 12
 *  sizeof(struct st1) = 12
    sizeof(struct st2) = 12
    sizeof(struct st3) = 16
    sizeof(struct st4) = 12
    sizeof(struct st5) = 44
    sizeof(struct st6) = 44
    sizeof(struct test) = 12
    sizeof(struct A) = 8
    sizeof(struct B) = 12
    sizeof(char) = 1
    sizeof(short) = 2
    sizeof(int) = 4
    sizeof(long) = 8
    sizeof(long long) = 8
    sizeof(float) = 4
    sizeof(double) = 8
    sizeof(ptr) = 8
    sizeof("Alan") = 5
    __INT_MAX__ = 2147483647
    __INT8_MAX__ = 127
    __LONG_MAX__ = 9223372036854775807
 * 
*/
int main()
{
    size_t psize;
    char *ptr = NULL;

    struct st1 *p = NULL;
    psize = sizeof(struct st1) + strlen("Alan");  
    p = malloc(psize);

    printf("psize = %ld, sizeof(*p) = %ld\n",psize,sizeof(*p)); 
    free(p);

    printf("sizeof(struct st1) = %ld\n",sizeof(struct st1));    

    printf("sizeof(struct st2) = %ld\n",sizeof(struct st2));

    printf("sizeof(struct st3) = %ld\n",sizeof(struct st3));

    printf("sizeof(struct st4) = %ld\n",sizeof(struct st4));

    printf("sizeof(struct st5) = %ld\n",sizeof(struct st5));

    printf("sizeof(struct st6) = %ld\n",sizeof(struct st6));

    printf("sizeof(struct st7) = %ld\n",sizeof(struct st7));

    printf("sizeof(struct st8) = %ld\n",sizeof(struct st8));

    printf("sizeof(struct test) = %ld\n",sizeof(struct test));

    printf("sizeof(struct A) = %ld\n",sizeof(struct A));

    printf("sizeof(struct B) = %ld\n",sizeof(struct B));

    printf("sizeof(char) = %ld\n", sizeof(char));

    printf("sizeof(short) = %ld\n", sizeof(short));

    printf("sizeof(int) = %ld\n", sizeof(int));

    printf("sizeof(long) = %ld\n", sizeof(long));

    printf("sizeof(long long) = %ld\n", sizeof(long long));

    printf("sizeof(float) = %ld\n", sizeof(float));

    printf("sizeof(double) = %ld\n", sizeof(double));

    printf("sizeof(ptr) = %ld\n", sizeof(ptr));

    printf("sizeof(\"Alan\") = %ld\n", sizeof("Alan"));

    printf("__INT_MAX__ = %ld\n", __INT_MAX__);

    printf("__INT8_MAX__ = %ld\n", __INT8_MAX__);

    printf("__LONG_MAX__ = %ld\n", __LONG_MAX__);


    exit(0);
}