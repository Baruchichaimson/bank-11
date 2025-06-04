#include <stdio.h>
 
/***************************
author:Baruch Haimson
reviewer:Tal Hindi
status:approved

***************************/
void Swap (int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main()
{
	int a = 5;
	int b = 7;
        printf("a = %d, b = %d\n", a, b);
	Swap(&a, &b);
        printf("a = %d, b = %d\n", a, b);
	return 0;
}
