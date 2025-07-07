#include <limits.h>
#include "bit_arr.h"

#define SIZE_BIT (sizeof(bit_arr_t) * CHAR_BIT)

/* ==================question 9 =========================================*/
/* ---- LUT for Counting Bits Set to 1 ---- */
static const unsigned char LUT_COUNT_ON_BITS[256] =
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

/* ---- LUT for Bit Mirror ---- */
static const unsigned char LUT_MIRROR[256] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
    0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
    0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
    0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
    0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
    0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
    0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
    0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
    0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
    0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
    0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
    0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
    0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
    0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
    0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
    0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
    0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

/* ==================question 9 =========================================*/

bit_arr_t BitArrSetAllOn(bit_arr_t bit_arr)
{
    (void)bit_arr;
    return ~(bit_arr_t)0;
}

bit_arr_t BitArrSetAllOff(bit_arr_t bit_arr)
{
    (void)bit_arr;
    return (bit_arr_t)0;
}

bit_arr_t BitArrSetBit(bit_arr_t bit_arr, size_t index, int value)
{
    if (index > SIZE_BIT)
    {
        return bit_arr; 
    }
    if (value)
    {
        bit_arr = bit_arr | ((bit_arr_t)1 << index);
    }
    else
    {
        bit_arr = bit_arr & ~((bit_arr_t)1 << index);
    }
    return bit_arr;
}

int BitArrGetBit(bit_arr_t bit_arr, size_t index)
{
	if (index > SIZE_BIT)
    {
        return -1; 
    }
	return ((bit_arr >> index) & 1); 
}

bit_arr_t BitArrSetOn(bit_arr_t bit_arr, size_t index)
{
	if (index > SIZE_BIT)
    {
        return bit_arr; 
    }
    bit_arr = bit_arr | ((bit_arr_t)1 << index);
    return bit_arr; 
}

bit_arr_t BitArrSetOff(bit_arr_t bit_arr, size_t index)
{
	if (index > SIZE_BIT)
    {
        return bit_arr; 
    }
    bit_arr = bit_arr & ~((bit_arr_t)1 << index);
    return bit_arr; 
}

bit_arr_t BitArrRotateLeft(bit_arr_t bit_arr, size_t shift)
{
	shift %= SIZE_BIT;
	if(shift == 0)
	{
		return bit_arr;
	}
	return (bit_arr << shift) | (bit_arr >> (SIZE_BIT - shift));
}

bit_arr_t BitArrRotateRight(bit_arr_t bit_arr, size_t shift)
{
	shift %= SIZE_BIT;
	if(shift == 0)
	{
		return bit_arr;
	}
	return (bit_arr >> shift) | (bit_arr << (SIZE_BIT - shift));
}

bit_arr_t BitArrFlipBit(bit_arr_t bit_arr, size_t index)
{
	if (index > SIZE_BIT)
    {
        return bit_arr; 
    }
	return bit_arr ^= (bit_arr_t)1 << index;
}

size_t BitArrCountOn(bit_arr_t bit_arr)
{
	/* div any group to half and sum it to is neighbord */
    bit_arr = (bit_arr & 0x5555555555555555) + ((bit_arr >> 1) & 0x5555555555555555); 
    bit_arr = (bit_arr & 0x3333333333333333) + ((bit_arr >> 2) & 0x3333333333333333); 
    bit_arr = (bit_arr & 0x0F0F0F0F0F0F0F0F) + ((bit_arr >> 4) & 0x0F0F0F0F0F0F0F0F); 
    bit_arr = (bit_arr & 0x00FF00FF00FF00FF) + ((bit_arr >> 8) & 0x00FF00FF00FF00FF);
    bit_arr = (bit_arr & 0x0000FFFF0000FFFF) + ((bit_arr >> 16) & 0x0000FFFF0000FFFF);
    bit_arr = (bit_arr & 0x00000000FFFFFFFF) + ((bit_arr >> 32) & 0x00000000FFFFFFFF);
     
    return bit_arr;
}

size_t BitArrCountOff(bit_arr_t bit_arr)
{
	    return SIZE_BIT - BitArrCountOn(bit_arr);
}
bit_arr_t BitArrMirror(bit_arr_t bit_arr)
{
    bit_arr = ((bit_arr & 0x5555555555555555ULL) << 1) | ((bit_arr & 0xAAAAAAAAAAAAAAAAULL) >> 1);
    bit_arr = ((bit_arr & 0x3333333333333333ULL) << 2) | ((bit_arr & 0xCCCCCCCCCCCCCCCCULL) >> 2);
    bit_arr = ((bit_arr & 0x0F0F0F0F0F0F0F0FULL) << 4) | ((bit_arr & 0xF0F0F0F0F0F0F0F0ULL) >> 4);
    bit_arr = ((bit_arr & 0x00FF00FF00FF00FFULL) << 8) | ((bit_arr & 0xFF00FF00FF00FF00ULL) >> 8);
    bit_arr = ((bit_arr & 0x0000FFFF0000FFFFULL) << 16) | ((bit_arr & 0xFFFF0000FFFF0000ULL) >> 16);
    bit_arr = ((bit_arr & 0x00000000FFFFFFFFULL) << 32) | ((bit_arr & 0xFFFFFFFF00000000ULL) >> 32);
    
    return bit_arr;
}
char *BitArrToString(bit_arr_t bit_arr, char* str)
{
	size_t i = 0;
	char bit;
	if(str == NULL)
	{
		return NULL;
	}
    for (i = 0; i < SIZE_BIT; ++i)
    {

    	bit = BitArrGetBit(bit_arr, i) ? '1' : '0';
        str[SIZE_BIT - 1 - i] = bit;
    }
    str[SIZE_BIT] = '\0';
    return str;
}

/* ==================question 9 =========================================*/
size_t BitArrCountOnLUT(bit_arr_t bit_arr)
{
	size_t i = 0;
    size_t count = 0;
    unsigned char *bytePtr = (unsigned char *)&bit_arr;
    for (i = 0; i < sizeof(bit_arr_t); ++i)
    {
        count += LUT_COUNT_ON_BITS[bytePtr[i]];
    }
    return count;
}
bit_arr_t BitArrMirrorLUT(bit_arr_t bit_arr)
{
    bit_arr_t result = 0;
    unsigned char *byte_ptr = (unsigned char *)&bit_arr;
    unsigned char *res_ptr = (unsigned char *)&result;
    size_t i;

    for (i = 0; i < sizeof(bit_arr_t); ++i)
    {
        res_ptr[i] = LUT_MIRROR[byte_ptr[sizeof(bit_arr_t) - 1 - i]];
    }

    return result;
}





