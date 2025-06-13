#ifndef __EXC_BIT_H__
#define __EXC_BIT_H__

#include <stddef.h>

long pow2(int x, int y);
int isPowerOfTwoLoop(unsigned int n);
int isPowerOfTwo(unsigned int n);
unsigned int AddOne(unsigned int n);
void PrintWithThreeBits(int* arr, size_t size);
unsigned char ByteMirrorLoop(unsigned char n);
unsigned char ByteMirror(unsigned char n);
int Checks2and6(unsigned char c);
int Checks2or6(unsigned char c);
int Swap3and5(unsigned char c);
unsigned int RoundDownTo16(unsigned int n);
void SwapInPlace(unsigned int x, unsigned int y);
int CountBitsLoop(int n);
int CountBits(int n);
void PrintFloatBits(float n); 

#endif 

