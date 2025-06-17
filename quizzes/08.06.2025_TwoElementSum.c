#include <stddef.h>

typedef struct Pair
{
    size_t index_1;
    size_t index_2;
} pair_ty;

int TwoElementsSum(const int numbers[], size_t size, int sum, pair_ty* pair)
{
    /* ### Write your code below this line ### */
    size_t i = 0;
	size_t j = 0;
	while(i < size)
    {
       pair->index_1 = j;
       pair->index_2 = size - i - 1; 
       if((numbers[pair->index_1] + numbers[pair->index_2]) > sum)
       {
           i++;
       }
       if((numbers[pair->index_1] + numbers[pair->index_2]) < sum)
       {
           j++;
       }  
       if((numbers[pair->index_1] + numbers[pair->index_2]) == sum)
       {
           return 0;
       }
    }
    return -1;
}

