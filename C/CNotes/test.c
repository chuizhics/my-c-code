#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node;


int main(int args, char *argv[])
{
    node *ptr = NULL;
    printf("%d----sizeof(node) = %ld, sizeof(*ptr) = %ld, sizeof(ptr) = %ld\n",__LINE__, sizeof(node), sizeof(*ptr), sizeof(ptr));
    ptr = malloc(sizeof(node));
    if(ptr == NULL)
        return -1;
    ptr->data = 0;
    ptr->next = NULL;

    node *test = NULL;
    printf("%d]----test = %p, &test = %p\n", __LINE__, test, &test);
    
    size_t x = (char *)ptr;
 
    printf("%d]----x = %ld\n", __LINE__, x);

    printf("%d]----ptr = %p, (char *)ptr = %p, (char *)ptr = %ld, sizeof(&(*ptr)) = %ld\n", __LINE__, ptr, (char *) ptr, (char *)ptr,sizeof(&(*ptr)));

    printf("%d]----ptr = 0x%016llx, (char *)ptr = 0x%016llx, (char *)ptr = %ld, sizeof(&(*ptr)) = %ld\n", __LINE__, ptr, (char *) ptr, (char *)ptr,sizeof(&(*ptr)));


    int i;
    int *p = NULL, *q = NULL;
    int arr[] = {1, 2, 3, 4, 5, 6};
    p = malloc(sizeof(arr));

    for (i = 0; i < sizeof(arr) / sizeof(*arr); i++)
    {
        p[i] = arr[i];
    }

    q = p;

    printf("&p = %p, p = %p, *p = %d\n", &p, p, *p);

    printf("&q = %p, q = %p, *q = %d\n", &q, q, *q);

    free(p);

    printf("&p = %p, p = %p, *p = %d\n", &p, p, *p);

    printf("&q = %p, q = %p, *q = %d\n", &q, q, *q);

    exit(0);
}