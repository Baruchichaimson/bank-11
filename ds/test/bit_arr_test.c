#include <stdio.h>
#include <string.h>
#include "bit_arr.h"

#define BUFFER_SIZE 65

/* ---- Utility to print test results ---- */
void PrintResult(const char *test_name, int expected, int actual)
{
    printf("%s:\n", test_name);
    printf("  Expected: %d\n", expected);
    printf("  Actual  : %d\n", actual);
    if (expected == actual)
    {
        printf("  [PASS]\n\n");
    }
    else
    {
        printf("  [FAIL]\n\n");
    }
}

void PrintStringResult(const char *test_name, const char *expected, const char *actual)
{
    printf("%s:\n", test_name);
    printf("  Expected: %s\n", expected);
    printf("  Actual  : %s\n", actual);
    if (strcmp(expected, actual) == 0)
    {
        printf("  [PASS]\n\n");
    }
    else
    {
        printf("  [FAIL]\n\n");
    }
}

/* ---- Test functions ---- */

void TestSetAndGet()
{
    printf("Running TestSetAndGet...\n");
    bit_arr_t bits = BitArrSetAllOff(0);
    bits = BitArrSetOn(bits, 0);
    bits = BitArrSetOn(bits, 5);
    bits = BitArrSetOn(bits, 63);

    PrintResult("Bit at index 0", 1, BitArrGetBit(bits, 0));
    PrintResult("Bit at index 5", 1, BitArrGetBit(bits, 5));
    PrintResult("Bit at index 63", 1, BitArrGetBit(bits, 63));

    bits = BitArrSetOff(bits, 5);
    PrintResult("Bit at index 5 after OFF", 0, BitArrGetBit(bits, 5));
}

void TestSetBit()
{
    printf("Running TestSetBit...\n");
    bit_arr_t bits = BitArrSetAllOff(0);
    bits = BitArrSetBit(bits, 3, 1);
    PrintResult("Bit at index 3 set to 1", 1, BitArrGetBit(bits, 3));
    bits = BitArrSetBit(bits, 3, 0);
    PrintResult("Bit at index 3 set to 0", 0, BitArrGetBit(bits, 3));
}

void TestFlipBit()
{
    printf("Running TestFlipBit...\n");
    bit_arr_t bits = BitArrSetAllOff(0);
    bits = BitArrFlipBit(bits, 4);
    PrintResult("Bit at index 4 after flip to 1", 1, BitArrGetBit(bits, 4));
    bits = BitArrFlipBit(bits, 4);
    PrintResult("Bit at index 4 after flip back to 0", 0, BitArrGetBit(bits, 4));
}

void TestRotate()
{
    printf("Running TestRotate...\n");
    bit_arr_t bits = 1;
    bits = BitArrRotateLeft(bits, 1);
    PrintResult("After RotateLeft(1), bit 1", 1, BitArrGetBit(bits, 1));
    bits = BitArrRotateRight(bits, 1);
    PrintResult("After RotateRight(1), bit 0", 1, BitArrGetBit(bits, 0));
}

void TestCount()
{
    printf("Running TestCount...\n");
    bit_arr_t bits = BitArrSetAllOff(0);
    bits = BitArrSetOn(bits, 0);
    bits = BitArrSetOn(bits, 1);
    bits = BitArrSetOn(bits, 63);
    PrintResult("CountOn", 3, BitArrCountOn(bits));
    PrintResult("CountOff", 61, BitArrCountOff(bits));
}

void TestMirror()
{
    printf("Running TestMirror...\n");
    bit_arr_t bits = 1;
    bit_arr_t mirrored = BitArrMirror(bits);
    PrintResult("Mirrored bit 63", 1, BitArrGetBit(mirrored, 63));
    PrintResult("Mirrored bit 0", 0, BitArrGetBit(mirrored, 0));
}

void TestToString()
{
    printf("Running TestToString...\n");
    bit_arr_t bits = BitArrSetAllOff(0);
    bits = BitArrSetOn(bits, 0);
    bits = BitArrSetOn(bits, 63);
    char buffer[BUFFER_SIZE];
    BitArrToString(bits, buffer);

    /* Build expected string */
    char expected[BUFFER_SIZE];
    memset(expected, '0', 64);
    expected[0] = '1';     /* MSB */
    expected[63] = '1';    /* LSB */
    expected[64] = '\0';

    PrintStringResult("BitArrToString", expected, buffer);
}

/* ---- Main ---- */

int main()
{
    TestSetAndGet();
    TestSetBit();
    TestFlipBit();
    TestRotate();
    TestCount();
    TestMirror();
    TestToString();

    printf("All tests completed.\n");
    return 0;
}

