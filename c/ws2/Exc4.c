#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/***************************
author:Baruch Haimson
reviewer:Tal Hindi
status:approved

***************************/

void SwapSizeT(size_t* a, size_t* b)
{
    size_t temp = *a;
    *a = *b;
    *b = temp;
}


void SwapSizeTPointers(size_t** a, size_t** b)
{
    SwapSizeT((size_t*) a, (size_t*) b);
}

int main ()
{
	 size_t x = 100;
	 size_t y = 200;
         size_t* px = &x;
         size_t* py = &y;
         printf("Before swap:\n");
    	 printf("px points to: %lu\n", (unsigned long)*px);
    	 printf("py points to: %lu\n", (unsigned long)*py);

    	 SwapSizeTPointers(&px, &py);

    	 printf("After swap:\n");
    	 printf("px points to: %lu\n", (unsigned long)*px);
    	 printf("py points to: %lu\n", (unsigned long)*py);
         return 0;
}
