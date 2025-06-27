#include <stdio.h>

int func(size_t a) 
{
	size_t i = 0;
    size_t count = 0;
    for (i = 0; i < 7; ++i) 
    {
        if (((a >> i) & 0x3) == 0x3) 
        {
            count++;
        }
    }
    return count;
}


void swap1(int* a, int* b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


int countset(long b) 
{
    size_t count = 0;
    while (b) 
    {
        b = b & (b - 1);
        count++;
    }
    return count;
}


void swap2(int* a, int* b) 
{
	if(*a != *b)
	{
		*a = *a ^ *b;
		*b = *a ^ *b;
		*a = *a ^ *b;
	}
}


void swap3(int* a, int* b) 
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
	
}


int main() 
{

    unsigned char byte = 0x76; /* 01110110 */
    printf("Testing func:\n");
    printf("Input: 0x%X (binary: 01110110)\n", byte);
    printf("Expected: 3, Got: %d\n\n", func(byte));

    int x = 5, y = 10;

    printf("Testing swap1:\n");
    printf("Before: x = %d, y = %d\n", x, y);
    swap1(&x, &y);
    printf("After:  x = %d, y = %d (Expected: 10, 5)\n\n", x, y);

    printf("Testing swap2:\n");
    swap2(&x, &y);
    printf("After:  x = %d, y = %d (Expected: 5, 10)\n\n", x, y);

    printf("Testing swap3:\n");
    swap3(&x, &y);
    printf("After:  x = %d, y = %d (Expected: 10, 5)\n\n", x, y);

    long num = 0x60; /* 000...01100000 */
    printf("Testing countset:\n");
    printf("Input: 0x%lX\n", num);
    printf("Expected: 2, Got: %d\n", countset(num));

    return 0;
}
