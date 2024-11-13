#include <stdio.h>
#include <stdlib.h>

int main()
{
    int arr[64], num;
    int base = 1, i = 0;
    while(1)
    {
        printf("Please enter a number:\n");
        scanf("%d", &num);
        printf("please enter base:\n");
        scanf("%d", &base);

        do
        {
            arr[i] = num % base;
            num = num / base;
            i++;
        } while (num != 0);

        for (; i > 0; i--)
        {
            if (arr[i - 1] >= 10)
            {
                printf("%c", 'A' + arr[i - 1] - 10);
            }
            else
                printf("%d", arr[i - 1]);
        }
        printf("\n");
    }
    exit(0);
}