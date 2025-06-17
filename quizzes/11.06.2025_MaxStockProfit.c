#include <stddef.h>

typedef struct Result
{
    size_t buy_index;
    size_t sell_index;
} result_ty;

result_ty MaxStockProfit(const int prices[], size_t size)
{
    size_t i = 0;
    size_t min_index;
    int maxProfit = 0;
    int min = prices[0];
    result_ty result = {0, 0};


    /* ### Write your code below this line ### */
    for(i = 0; i < size; i++)
    {
        if(prices[i] < min)
        {
            min = prices[i];
            min_index = i;
        }
		else if ((prices[i] - min) > maxProfit) 
        {
            maxProfit = prices[i] - min;
            result.buy_index = min_index;
            result.sell_index = i;
        }
    }
    
    return result;
}

