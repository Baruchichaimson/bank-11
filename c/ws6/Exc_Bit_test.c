#include <stdio.h>
#include "Exc_Bit.h"

int main()
{
    int arr[7] = {5, 7, 12, 13, 14, 15, 8};
    printf("%ld\n", pow2(3, 5));

    printf("isPowerOfTwoLoop:\n");
    printf("%d\n", isPowerOfTwoLoop(0));
    printf("%d\n", isPowerOfTwoLoop(5));
    printf("%d\n", isPowerOfTwoLoop(8));

    printf("*************************\n");
    printf("isPowerOfTwo:\n");
    printf("%d\n", isPowerOfTwo(0));
    printf("%d\n", isPowerOfTwo(15));
    printf("%d\n", isPowerOfTwo(18));
    printf("%d\n", isPowerOfTwo(32));

    printf("**********ADD ONE************\n");
    printf("%d\n", AddOne(32));
    printf("%d\n", AddOne(0));
    printf("%d\n", AddOne(1));
    printf("%d\n", AddOne(7));
    printf("%d\n", AddOne(5));

    printf("**********printWithThreeBits************\n");
    PrintWithThreeBits(arr, 7);
    
    printf("**********MirrorByteLoop************\n");
    printf("%d\n", ByteMirrorLoop(6));
    printf("%d\n", ByteMirrorLoop(13));
    
    printf("**********MirrorByte************\n");
	printf("%d\n", ByteMirror(6));
	printf("%d\n", ByteMirror(13));
	
	printf("**********2nd -and- 6nd************\n");
	printf("%d\n", Checks2and6(34));
	printf("%d\n", Checks2and6(4));
	
	printf("**********2nd -or- 6nd************\n");
	printf("%d\n", Checks2or6(32));
	printf("%d\n", Checks2or6(2));
	
	printf("**********3nd -swap- 5nd************\n");
	printf("%d\n", Swap3and5(4));
	printf("%d\n", Swap3and5(16));
	printf("%d\n", Swap3and5(0));
	printf("%d\n", Swap3and5(55));	
	
	printf("**********RoundDownTo16************\n");
	printf("%d\n" , RoundDownTo16(5));
	printf("%d\n" , RoundDownTo16(16));
	printf("%d\n" , RoundDownTo16(32));
	printf("%d\n" , RoundDownTo16(80));
	
	printf("**********SwapInPlace************\n");
	SwapInPlace(3, 5);
	SwapInPlace(3.6, 5.4);
	SwapInPlace(3.1, 5.2);
	SwapInPlace(5, 5);
	SwapInPlace(4, -5);
	SwapInPlace(1.0, -1);
	
	printf("**********CountBitsLoop************\n");
	printf("%d\n", CountBitsLoop(6));
	printf("%d\n", CountBitsLoop(13));
	
	printf("**********CountBits************\n");
	printf("%d\n", CountBits(6));
	printf("%d\n", CountBits(13));
	
	printf("**********PrintFloatBits************\n");
	PrintFloatBits(3.16);
	PrintFloatBits(3.75);
	PrintFloatBits(5.40);
	
    return 0;
}

