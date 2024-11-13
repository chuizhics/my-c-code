#include <stdio.h>
#include <stdlib.h>
// simple interest on A 
#define ASI 0.10
// A's investment amount
#define AI 100
//  B's Compound Interest
#define BCI 0.05

#define BI 100

//百钱买百鸡
#define MONEY 100

#define AMOUNT 100

#define ROOSTER 5

#define HEN 3

#define CHICK3 1


#define CUBE(NUM) ((NUM)* (NUM) * (NUM)) 

#define FN 50

#define A 'G'
#define Z 'X'


#define PRIME_MAX 1000


static void invest(void)
{



    return;
}


static void count(void)
{
    int even_num = 0 ;
    int odd_num = 0;
    double avg_en = 0.0;
    double avg_on = 0.0;
    int i = 0;
    printf("[%s]Please enter:\n",__FUNCTION__);
    while (1)
    {   
        if(scanf("%d", &i) != 1)
        {
            fprintf(stderr,"EINVAL\n");
            exit(1);
        }
        if(i != 0)
        {
            switch (i % 2)
            {
            case 0:
                even_num++;
                avg_en = (avg_en * (even_num- 1) + i) * 1.0/ even_num;
                break;
            case 1:
                odd_num++;
                avg_on = (avg_on * (odd_num- 1) + i) * 1.0/ odd_num;
                break;
            default:
                break;
            }
            
        }
        else if (i == 0)
            break;
        else
        {
            printf("[%s:%d]logic error!\n", __FUNCTION__,__LINE__);
        }
    }
    
    printf("number of even numbers = %d\taverage of even numbers = %f\t\nnumber of odd numbers = %d\taverage of odd numbers = %f\t\n",even_num,avg_en,odd_num,avg_on);

    return;
}


static void cvowels(void)
{
    printf("[%s]Please enter:\n",__FUNCTION__);
    int vow_num = 0;
    char ch;
    while (scanf("%c", &ch))
    {
        switch (ch)
        {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                vow_num++;
                break;
            default:
                break;
        }
    }
    printf("The number of vowels in the input character is %d\n", vow_num);

    return;
}


static void fibo(void)
{
    long f0 = 1 , f1 = 1;
    int i = 0;
    long c = 0;
    for( ;i < FN; i++)
    {
        if (i == 0) 
            printf("%ld", f0);
        else if(i == 1)
            printf(",%ld", f1);
        else
        {
            c = f1 + f0;
            f0 = f1;
            f1 = c;
            printf(",%ld",c);
        }
    }
    printf("\n");
    return;
} 

static void multiplicationTable(void)
{
    int i, j;
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j <= i; j++)
        {
            printf("%d x %d = %d\t", j + 1, i + 1, (j + 1) * (i + 1));
        }
        printf("\n");
    }
    printf("\n");
    return;
}

static void chicken(void)
{
    int rooster = 0;
    int hen = 0;
    int chick = 0;
    int cost = 0;

    for(chick = 0;  chick * CHICK3 / 3 <= MONEY; chick+=3)
    {
        cost = chick * CHICK3 / 3;
        for(rooster = 0; rooster + chick <= AMOUNT && cost <= MONEY; rooster++)
        {
            hen = AMOUNT - rooster - chick;
            cost = hen * HEN + chick * CHICK3 / 3 + rooster * ROOSTER;
            if(cost == MONEY)
                printf("rooster = %d, hen = %d, chick = %d\n", rooster, hen, chick);
        }
    }
    return;
}


static void daffodils(void)
{   
    printf("The number of all daffodils within 1000 is:\n");
    int daffodils;
    for(daffodils = 100; daffodils < 1000; daffodils++)
    {   
        if(daffodils == CUBE(daffodils/100) + CUBE((daffodils/10)%10) + CUBE(daffodils%10))
            printf("%d ",daffodils);
    }
    printf("\n");
    return;
}

static void prime(void)
{
    printf("All prime numbers up to 1000 are:\n");
    int prime;
    int i;
    int tatol = 1;
    printf("2 ");
    for(prime = 3; prime <= PRIME_MAX; prime += 2)
    {
        for(i = 3; i * i <= prime; i++)
        {
            if(prime % i == 0)
                break;
        }
        if(i * i > prime)
        {
            tatol++;
            printf("%d ", prime);
        }
    }
    printf("\n");
    printf("total = %d\n",tatol);
    return;
}

static void plot(void)
{   
    int i = A;
    int j = Z;
    int t = 0;
    for( ; i <= j; i++)
    {
        for(t = j - i; t >= 0; t--)
        {
            printf("%c", i + (j-i-t));
        }
        printf("\n");
    }
    return;
}



int main()
{

    invest();
    count();
    //cvowels();
    fibo();
    multiplicationTable();
    chicken();
    daffodils();
    prime();

    plot();


    exit(0);
}