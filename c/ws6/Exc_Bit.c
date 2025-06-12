#include <stdio.h>
#include "Exc_Bit.h"

long pow2(int x, int y)
{
    return (long)x << y;
}

int isPowerOfTwoLoop(unsigned int n)
{
    size_t count = 0;
    while (n > 0)
    {
        if (n & 1)
        {
            count++;
        }
        n = n >> 1;
    }
    return (1 == count);
}

int isPowerOfTwo(unsigned int n)
{
    if (n == 0)
        return 0;
    return !(n & (n - 1));
}

unsigned int AddOne(unsigned int n)
{
    int carry = 1;
    int sum = 0;
    while (carry != 0)
    {
        sum = n ^ carry;
        carry = (n & carry) << 1;
        n = sum;
    }
    return n;
}
void PrintWithThreeBits(int* arr, size_t size)
{
	size_t i;
	size_t count = 0;
    unsigned int num = 0;
    for (i = 0; i < size; i++)
    {
        count = 0;
	    num = arr[i];
        while (num > 0)
        {
            if (num & 1)
            {
                count++;
            }
            num >>= 1;
        }

        if (3 == count)
        {
            printf("%d\n", arr[i]);
        }
    }
}
unsigned int ByteMirrorLoop(unsigned int n)
{
	unsigned int result = 0;
    for (int i = 0; i < 32; ++i)
    {
        result <<= 1;
        result |= (n & 1);
        n >>= 1;
    }
    return 1;
	
}
unsigned int ByteMirror(unsigned int n)
{
	n = (((n >> 1) & 0x55555555) | (n & 0x55555555) << 1);
	n = (((n >> 2) & 0x33333333) | (n & 0x33333333) << 2);
	n = (((n >> 4) & 0x0F0F0F0F) | (n & 0x0F0F0F0F) << 4);
	n = (((n >> 8) & 0x00FF0FF) | (n & 0x00FF00FF) << 8);
	n = (n >> 16) | (n << 16); 
	return n;
}

int Checks2and6(unsigned char c)
{
	return (((c >> 1) & 1) & ((c >> 5) & 1));
}
int Checks2or6(unsigned char c)
{
	return (((c >> 1) & 1) | ((c >> 5) & 1));
}
int Swap3and5(unsigned char c)
{
	unsigned char final;
	unsigned char z;
	z  = ((c >> 4) & 1) ^ ((c >> 2) & 1);
	final = (c ^ ((z << 2) | (z << 4)));
	return final;
}
unsigned int RoundDownTo16(unsigned int n)
{
	n = n & 0xFFFFFFF0;
	n = n >> 4;
	return n;
}
void SwapInPlace(unsigned int x, unsigned int y)
{
	x = x ^ y;
	y = x ^ y;
	x = x ^ y;
	printf("%d - %d\n", x, y);
	/* not work for the same numbers and for float numbers */
}
CountBits()


