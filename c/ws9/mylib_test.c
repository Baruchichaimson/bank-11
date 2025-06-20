#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "mylib.h" 
 

void TestMemset()
{
    printf("Testing MyMemset:\n");

    char buffer1[20];
    char buffer2[20];

    memset(buffer1, 'A', sizeof(buffer1));
    MyMemset(buffer2, 'A', sizeof(buffer2));

    assert(memcmp(buffer1, buffer2, sizeof(buffer1)) == 0);
    printf("passed\n");
}

void TestMemcpy()
{
    printf("Testing MyMemcpy:\n");

    char src[] = "Hello world!";
    char dest1[50];
    char dest2[50];

    memcpy(dest1, src, sizeof(src));
    MyMemcpy(dest2, src, sizeof(src));

    assert(memcmp(dest1, dest2, sizeof(src)) == 0);
    printf("passed\n");
}

void TestMemmoveOverlap()
{
    printf("Testing MyMemmove with overlapping:\n");

    char buffer1[100] = "1234567890";
    char buffer2[100] = "1234567890";

    memmove(buffer1 + 2, buffer1, 4);
    MyMemmove(buffer2 + 2, buffer2, 4);

    assert(memcmp(buffer1, buffer2, sizeof(buffer1)) == 0);
    printf("passed\n");
}

void TestMemmoveNoOverlap()
{
    printf("Testing MyMemmove with no overlap:\n");

    char src[] = "abcdef";
    char dest1[20];
    char dest2[20];

    memmove(dest1, src, sizeof(src));
    MyMemmove(dest2, src, sizeof(src));

    assert(memcmp(dest1, dest2, sizeof(src)) == 0);
    printf("passed\n");
}

int main()
{
	/*  
		gd mylib_test.c -L. -lmylib  - gd mylib.c mylib_test.c
		and this take the function from the static libary 
		and the .h file just to check the title of the function 
		but dont give the implementation of them
	*/
	
    TestMemset();
    TestMemcpy();
    TestMemmoveOverlap();
    TestMemmoveNoOverlap();
    printf("\nAll tests success\n");
    return 0;
}

