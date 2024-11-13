#include <stdio.h>
#include <stdlib.h>

#define STRSIZE 32

int main()
{
    int i, ret;
    float f;
    char ch , str[STRSIZE];

    printf("Please enter:\n");

    scanf("%d", &i);
    scanf("%*c%c",&ch);

    printf("i = %d, ch = %d\n", i , ch);


#if 0
    while (1)
    {
        ret = scanf("%d", &i);
        if (ret != 1)
        {
            printf("Enter error!\n");
            break;
        }
        printf("i = %d\n", i);

        
    }
    
#endif 
//    scanf("%s",str);   危险

//    printf("str = %s\n", str);


    // scanf("%d%f",&i,&f);

    // printf("i = %d\n", i);

    // printf("f = %f\n", f);


    exit(0);
}