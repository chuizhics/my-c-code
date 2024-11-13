#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  MASS_OF_A_WATER_MOLECULE  (3.0E-23)

#define ONE_QUART_MASS_OF_WATER 950



static void area(void)
{
    float a,b,c;
    printf("Please enter for a,b,c:\n");

    if(scanf("%f%f%f",&a,&b,&c) != 3 || ((b+c) <= a) || ((a+b) <=c) || ((a +c ) <= b))
    {
        //printf("[%s:%d]input error!\n", __FUNCTION__,__LINE__);
        fprintf(stderr, "EINVAL\n");
        exit(1);
    }

    double p = 1.0 / 2 * (a + b + c);

    double area = sqrt(p*(p-a)*(p-b)*(p-c));

    printf("The area of the triangle is %f\n", area);

    return;
}

static void water(void)
{
    float quarts_of_water;
    
    printf("Please enter for quarts_of_water:\n");

    scanf("%f", &quarts_of_water);

    double result = (quarts_of_water * ONE_QUART_MASS_OF_WATER) * 1.0 / MASS_OF_A_WATER_MOLECULE;

    printf("There are approximately %e water molecules in %f quarts of water.\n",result, quarts_of_water);
    
    return;
}

int main()
{
   // water();

   // area();

    exit(0);
}