/************************************
 * Exercise: cmp_sorting (bubble, insertion, selection)
 * Date: 05/08/25
 * Developer: Baruch Haimson
 * Reviewer: guy
 * Status: Approved
 ************************************/
 
#include <assert.h> /* assert */

#include "cmp_sorting.h" /* API */

/******** helper function ************/

static void swap(int* a, int* b);

/*************************************/

void SortingBubble(int* arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	int swaping = 0;
	
	assert(arr);
	
	for(i = 0; i < size - 1; i++)
	{
		swaping = 0;
		for(j = 0; j < size - 1 - i; j++)
		{
			if(arr[j] > arr[j + 1])
			{
				swap(&arr[j], &arr[j + 1]);
				swaping = 1;
			}
		}
		if(!swaping)
		{
			return;
		}
	}
}

void SortingInsertion(int* arr, size_t size)
{
	size_t i = 0;
    size_t j = 0;
    int key = 0;

    assert(arr);

    for (i = 1; i < size; ++i)
    {
        key = arr[i];
        j = i;

        while (j > 0 && arr[j - 1] > key)
        {
            arr[j] = arr[j - 1];
            --j;
        }

        arr[j] = key;
    }
}
void SortingSelection(int* arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	size_t min = 0;
	
	assert(arr);
	
	for (i = 0; i < size - 1; i++)
	{
		min = i;

		for (j = i + 1; j < size; j++)
		{

		    if (arr[j] < arr[min])
		    {
		        min = j;
		    }
		}

		if(min != i) 
		{
		    swap(&arr[i], &arr[min]);
		}
	}
}

/************** helper function **********************/
static void swap(int* a, int* b)
{
	int temp = 0;
	
	temp = *a;
	*a = *b;
	*b = temp;
}
