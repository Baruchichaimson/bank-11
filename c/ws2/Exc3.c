#include <stdio.h>
#include <stdlib.h>

/***************************
author:Baruch Haimson
reviewer:Tal Hindi
status:approved

***************************/

int main ()
{
    static int s_i = 7;
    int i = 7;
    int* ptr = &i;
    int* ptr2 = (int *)malloc(sizeof(int));

    printf("Address of static s_i: %p\n", (void*)&s_i);
    printf("Address of auto i: %p\n", (void*)&i);
    printf("Address of ptr (on stack): %p\n", (void*)&ptr);
    printf("Address stored in ptr (points to i): %p\n", (void*)ptr);
    printf("Address of ptr2 (on stack): %p\n", (void*)&ptr2);
    printf("Address stored in ptr2 (points to heap): %p\n", (void*)ptr2);

    if (ptr)
    {
        int** ptr3 = &ptr;
        printf("Address of ptr3 (on stack inside block): %p\n", (void*)&ptr3);
	printf("Value of ptr3 (address of ptr): %p\n", (void*)ptr3);
	printf("Value of ptr3 (address of i): %p\n", (void*)*ptr3);
	printf("Value of **ptr3 (value of i): %d\n", **ptr3);
    }

    free(ptr2);
    return 0;
}
