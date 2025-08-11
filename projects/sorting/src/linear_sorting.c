/************************************
 * Exercise: linear sorting (counting, radix)
 * Date: 06/08/25
 * Developer: Baruch Haimson
 * Reviewer: guy
 * Status: Approved
 ************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* size_t */
#include <string.h> /* memcpy */
#include <sys/types.h>  /* ssize_t */

#include "linear_sorting.h" /* API */

static void SortCountingByDigit(int* arr, size_t size, int digit);

void SortCounting(int* arr, size_t size)
{
	size_t i = 0;
	ssize_t j = 0;
	size_t range = 0;
	int min = arr[0];
    int max = arr[0];
	int* countArr;
	int* result;

	assert(arr);
	
	for(i = 0; i< size; i++)
	{
		if(arr[i] > max)
		{
			max = arr[i];
		}
		if(arr[i] < min)
		{
			min = arr[i];
		}
	}
	range = max - min + 1;
	
	countArr = (int*)calloc(range , sizeof(int)); 
	if(!countArr)
	{
		return;
	}
	
	
    result = (int*)malloc(size * sizeof(int));
    if(!result)
	{
		free(countArr);
		return;
	}
	
	for(i = 0; i < size; i++)
	{
		countArr[arr[i] - min]++;
	}
	
	for(i = 1; i < range; i++)
	{
		countArr[i] += countArr[i - 1];
	}
	
	for(j = size - 1; j >= 0; --j)
	{
		result[(countArr[arr[j] - min]) - 1] = arr[j];
		countArr[arr[j] - min]--;

	}
	
	memcpy(arr, result, size * sizeof(int));
	
	free(countArr);
	free(result);
}

void SortRadix(int* arr, size_t size)
{
	size_t i;
	int digit = 1;
    int max = arr[0];

    assert(arr);

    for (i = 1; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }

    for (digit = 1; max / digit > 0; digit *= 10)
    {
        SortCountingByDigit(arr, size, digit);
    }
}

static void SortCountingByDigit(int* arr, size_t size, int exp)
{
    int* result;
    int countArr[10] = {0};
    size_t i = 0;
    int digit = 0;
    
    assert(arr);
    
    result = (int*)malloc(size * sizeof(int));
    if(!result)
	{
		return;
	}

    for (i = 0; i < size; i++)
    {
        digit = (arr[i] / exp) % 10;
        countArr[digit]++;
    }

    for (i = 1; i < 10; i++)
    {
        countArr[i] += countArr[i - 1];
    }

    for (i = size; i > 0; i--)
    {
        digit = (arr[i - 1] / exp) % 10;
        result[countArr[digit] - 1] = arr[i - 1];
        countArr[digit]--;
    }

    memcpy(arr, result, size * sizeof(int));
    
    free(result);
}












