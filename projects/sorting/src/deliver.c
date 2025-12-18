#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    int distance;
    int count;
} Order;

int compare(const void* a, const void* b)
{
    Order* o1 = (Order*)a;
    Order* o2 = (Order*)b;

    if (o1->distance != o2->distance)
    {
        return o1->distance - o2->distance;
    }

    return o1->count - o2->count;
}

void swap(int* a , int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
 
void bubblesort(int* D, int* C, int n)
{
    int swaped = 0;
    int i = 0;
    int j = 0;
    for(i = 0; i < n; i++)
    {
        swaped = 0;
        for(j = 0; j < n - 1 - i; j++)
        {
            
            if(D[j] > D[j + 1])
            {  
                swap(&D[j], &D[j + 1]);
                swap(&C[j], &C[j + 1]);
                swaped = 1;
            }   
            
        }
        if(!swaped)
        {
            break;
        }
        
    }
    for(i = 0; i < n; i++)
    {
        printf("%d - %d\n", C[i],D[i]);
    }

}
int delivers(int D[], int C[], int n, int P)
{
    int i = 0;
    int sell = 0;
    Order* orders = malloc(n * sizeof(Order));
    if (orders == NULL)
    {
        return 0; 
    }

    for(i = 0; i < n; i++)
    {
        orders[i].distance = D[i];
        orders[i].count = C[i];
    }

    qsort(orders, n, sizeof(Order), compare);
   
    for(i = 0; i < n; i++)
    {
        if(P >= orders[i].count)
        {
            P -= orders[i].count;
            sell++;
        }
        else
        {
            break;
        }
    }
    free(orders);
    return sell;

}

int main()
{
    int result = 0;
    int D[] = {2, 3, 3, 1};
    int C[] = {7, 5, 2, 1};
    result = delivers(D, C, 4, 10);
    printf("result : %d\n", result);
    return 0;
}