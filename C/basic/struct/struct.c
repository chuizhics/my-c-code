#include <stdio.h>
#include <stdlib.h>

#define NAMESIZE 32

struct student_st
{
    int id;
 //F    char name[];

    char name[NAMESIZE];
 //   char* name;
    struct birth_st
    {
        int year;
        int month;
        int day;
    } birth;
    float math;
    float English;
}__attribute__((packed));

void print_stu(struct student_st *pstu)
{
    printf("%d, %s, %d-%d-%d, %.1f, %.1f\n",pstu->id,pstu->name,pstu->birth.year,pstu->birth.month,pstu->birth.day,pstu->math,pstu->English);
}


int main()
{
    struct student_st stu = {10010, "Alan", {1989,6,4}, 97.5, 99.5};
   //F struct student_st *pstu = stu;
    struct student_st *pstu = &stu;

    print_stu(pstu);

    printf("sizeof(stu)) = %ld\n",sizeof(stu));
    printf("sizeof(pstu)) = %ld\n",sizeof(pstu));

    exit(0);
}