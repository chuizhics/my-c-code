#include <stdio.h>
#include <stdlib.h>


int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
    int i,j;
    *returnSize = 2;
    int *returnArr = malloc((*returnSize) * sizeof(int));
    for(i = 0; i < numsSize - 1; i++)
    {
        for(j = i + 1; j < numsSize; j++)
        {
            if(nums[i] + nums[j] == target)
            {
                returnArr[0] = i;
                returnArr[1] = j;
                return returnArr;
            }
        }
    }    
    return NULL;
}

void print_arr(int *ptr, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

int main()
{
    int nums[] = {2,7,9,11};
    int target = 9;
    int *returnSize;
    int *returnArr;
    returnArr = twoSum(nums, sizeof(nums)/sizeof(*nums), target, returnSize);
    
    print_arr(returnArr, *returnSize);
    
    exit(0);
}