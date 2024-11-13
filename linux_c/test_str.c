#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str1[32] = {'h','e','l','l','\0'};

    char str2[] = {'w','o','r','l','d','\0'};

    printf("str1=%s,str2=%s\n", str1, str2);

    printf("strcat(str1,str2) = %s\n",strcat(str1,str2));

    printf("str1=%s,str2=%s\n", str1, str2);

    char *pp =NULL;
   // F printf("pp=%d\n",strlen(pp)); segment fault  

    char str4[4] = {};


    /**
     * str4 = 
     * str4 = AB
     * str[3] = 
     * str4 = QB
     * 
    **/
    printf("str4 = %s\n", str4);

 // F   strncat(str4,"ABCDEFG",4);  //dangerous
    strncat(str4,"ABCDEFG",2);


    printf("str4 = %s\n", str4);  // 

    printf("str[3] = %c\n", str4[3]);

    //strncpy(str4,"Q742563", 1);  // strncpy 不会自动添加'\0'  strcpy会自动添加'\0' 但是可能越界
                                  // strncat ..........       strcat .......................

    strcpy(str4,"Q");

    printf("str4 = %s\n",str4);

   // strcat(str4+1,"HJKL");   //dangerous

 //   printf("str4 = %s\n",str4);  

    char str5[5] = {'A','B','\0'};

    // strcat(str5+1,"EDF");

    // printf("str5=%s\n",str5);

    strcpy(str5+1,"EDF");

    printf("str5=%s\n",str5);


    exit(0);
}