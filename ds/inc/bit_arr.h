#ifndef __ILRD_BIT_ARR__
#define __ILRD_BIT_ARR__

#include <stddef.h>

typedef size_t bit_arr_t;

/*
 * Function: BitArrSetAllOn
 * Input: A bit array (bit_arr_t) value.
 * Operation: Sets all bits in the array to 1.
 * Return: A new bit array with all bits turned on (set to 1).
*/
bit_arr_t BitArrSetAllOn(bit_arr_t bit_arr);

/*
 * Function: BitArrSetAllOff
 * Input: A bit array (bit_arr_t) value.
 * Operation: Clears all bits in the array (sets them to 0).
 * Return: A new bit array with all bits turned off (set to 0).
*/
bit_arr_t BitArrSetAllOff(bit_arr_t bit_arr);

/** ----
 * Function: BitArrSetBit
 * Input: 
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - index: The bit position to modify (0-based index).
 *     - value: Either 0 (turn off) or 1 (turn on).
 * Operation: Sets the bit at the given index to the specified value.
 * Return: A new bit array with the updated bit value at the given index.
---- */
bit_arr_t BitArrSetBit(bit_arr_t bit_arr, size_t index, int value);

/*
 * Function: BitArrGetBit
 * Input: 
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - index: The bit position to check.
 * Operation: Retrieves the value of the bit at the given index.
 * Return: 1 if the bit is set (on), 0 if the bit is clear (off).
*/
int BitArrGetBit(bit_arr_t bit_arr, size_t index);

/*
 * Function: BitArrSetOn
 * Input: 
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - index: The bit position to set.
 * Operation: Sets the bit at the given index to 1 (on).
 * Return: A new bit array with the specified bit turned on.
*/
bit_arr_t BitArrSetOn(bit_arr_t bit_arr, size_t index);

/*
 * Function: BitArrSetOff
 * Input: 
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - index: The bit position to clear.
 * Operation: Clears the bit at the given index to 0 (off).
 * Return: A new bit array with the specified bit turned off.
*/
bit_arr_t BitArrSetOff(bit_arr_t bit_arr, size_t index);

/*
 * Function: BitArrRotateLeft
 * Input: 
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - shift: Number of positions to rotate to the left.
 * Operation: Performs a circular left shift on the bit array.
 * Return: A new bit array after rotating bits to the left by 'shift' positions.
*/
bit_arr_t BitArrRotateLeft(bit_arr_t bit_arr, size_t shift);

/*
 * Function: BitArrRotateRight
 * Input: 
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - shift: Number of positions to rotate to the right.
 * Operation: Performs a circular right shift on the bit array.
 * Return: A new bit array after rotating bits to the right by 'shift' positions.
*/
bit_arr_t BitArrRotateRight(bit_arr_t bit_arr, size_t shift);

/*
 * Function: BitArrFlipBit
 * Input: 
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - index: The bit position to flip.
 * Operation: Toggles (inverts) the bit at the given index.
 * Return: A new bit array with the specified bit flipped.
*/
bit_arr_t BitArrFlipBit(bit_arr_t bit_arr, size_t index);

/*
 * Function: BitArrCountOn
 * Input: A bit array (bit_arr_t) value.
 * Operation: Counts how many bits are set to 1 (on).
 * Return: The total number of bits that are turned on.
*/
size_t BitArrCountOn(bit_arr_t bit_arr);

/*
 * Function: BitArrCountOff
 * Input: A bit array (bit_arr_t) value.
 * Operation: Counts how many bits are set to 0 (off).
 * Return: The total number of bits that are turned off.
*/
size_t BitArrCountOff(bit_arr_t bit_arr);

/*
 * Function: BitArrMirror
 * Input: A bit array (bit_arr_t) value.
 * Operation: Reverses the order of bits (bitwise mirror).
 *            For example, the least significant bit becomes the most significant.
 * Return: A new bit array with all bits mirrored.
*/
bit_arr_t BitArrMirror(bit_arr_t bit_arr);

/*
 * Function: BitArrToString
 * Input:
 *     - bit_arr: A bit array (bit_arr_t) value.
 *     - str: A pointer to a character array (must be large enough to hold all bits + null terminator).
 * Operation: Converts the bit array into a binary string representation of '0's and '1's.
 * Return: A pointer to the input character array containing the binary string.
*/
char *BitArrToString(bit_arr_t bit_arr, char* str);

#endif /* _ILRD_BIT_ARR_ */

