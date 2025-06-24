#include <stdio.h>
#include <string.h>
#include "Exc2.h"

static int num = 1
#define IS_LITTLE_ENDIAN()  (*(char*)&num == 1)

void TestAtoi(const char* str)
{
    int expected;
    int actual;
    expected = atoi(str);
    actual = atoi10(str);
    printf("Test atoi10: '%s' return %d ,expected is: %d\n", str, actual, expected);
}

int main()
{
	int res;
	const char* a;
	const char* b;
	const char* c;
	char buf[32];
	printf("========Atoi=========\n");
    TestAtoi("123");
    TestAtoi("-42");
    TestAtoi("   +99");
    TestAtoi("0");

	printf("========Itoa=========\n");
	
    itoa10(0, buf);
    printf("itoa10(0): %s\n", buf);
    itoa10(123, buf);
    printf("itoa10(123): %s\n", buf);
    itoa10(-456, buf);
    printf("itoa10(-456): %s\n", buf);

	printf("========ItoaBase=========\n");
	
    ItoaBase(37, buf, 36);
    printf("itoa_base(37, 36): %s (expected: 11)\n", buf);
    ItoaBase(255, buf, 16);
    printf("itoa_base(255, 16): %s (expected: FF)\n", buf);
    ItoaBase(10, buf, 2);
    printf("itoa_base(10, 2): %s (expected: 1010)\n", buf);
    ItoaBase(12345, buf, 10);
    printf("itoa_base(12345, 10): %s (expected: 12345)\n", buf);
    ItoaBase(0, buf, 10);
    printf("itoa_base(0, 10): %s (expected: 0)\n", buf);
    ItoaBase(-42, buf, 10);
    printf("itoa_base(-42, 10): %s (expected: -42)\n", buf);
    ItoaBase(-42, buf, 16);
    printf("itoa_base(-42, 16): %s \n", buf);
    
    printf("======== AtoiBase =========\n");

	res = AtoiBase("11", 36);
	printf("AtoiBase(\"11\", 36): %d (expected: 37)\n", res);

	res = AtoiBase("FF", 16);
	printf("AtoiBase(\"FF\", 16): %d (expected: 255)\n", res);

	res = AtoiBase("1010", 2);
	printf("AtoiBase(\"1010\", 2): %d (expected: 10)\n", res);

	res = AtoiBase("12345", 10);
	printf("AtoiBase(\"12345\", 10): %d (expected: 12345)\n", res);

	res = AtoiBase("0", 10);
	printf("AtoiBase(\"0\", 10): %d (expected: 0)\n", res);

	res = AtoiBase("-42", 10);
	printf("AtoiBase(\"-42\", 10): %d (expected: -42)\n", res);

	res = AtoiBase("2A", 16);
	printf("AtoiBase(\"2A\", 16): %d (expected: 42)\n", res);

	res = AtoiBase("z", 36);
	printf("AtoiBase(\"z\", 36): %d (expected: 35)\n", res);

	res = AtoiBase("10", 8);
	printf("AtoiBase(\"10\", 8): %d (expected: 8)\n", res);

	res = AtoiBase("-101", 2);
	printf("AtoiBase(\"-101\", 2): %d (expected: -5)\n", res);

	res = AtoiBase("G", 16);
	printf("AtoiBase(\"G\", 16): %d (expected: 0)\n", res);

	res = AtoiBase("2", 2);
	printf("AtoiBase(\"2\", 2): %d (expected: 0)\n", res);

    
	printf("========IsLittleEndian=========\n");
	
	a = "abcdefg";
    b = "bdfhij";
    c = "z";
    printf("Expected output: bdf\nActual output: ");
    PrintUniqueChars(a, b, c);

    a = "hello world";
    b = "hold";
    c = "x";
    printf("Expected output: dhlo\nActual output: ");
    PrintUniqueChars(a, b, c);

    a = "ABCabc123";
    b = "abc123";
    c = "3";
    printf("Expected output: abc12\nActual output: ");
    PrintUniqueChars(a, b, c);
	
	printf("========IsLittleEndian=========\n");
	
	if (IsLittleEndian())
    {
        printf("System is Little Endian\n");
    }
    else
    {
        printf("System is Big Endian\n");
    }
    printf("========IS_LITTLE_ENDIAN_MACRO=========\n");
    
	if (IS_LITTLE_ENDIAN())
    {
        printf("System is Little Endian (macro)\n");
    }
    else
    {
        printf("System is Big Endian (macro)\n");
    }

    return 0;
}
