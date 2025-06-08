#include <stdio.h>
#include <stdlib.h>

/***************************
 Exercise:  WS3
 Date: 	    05/08/25
 Developer: Baruch Haimson
 Reviewer:  daniel
 Status:    Approved ; suggestion - delete return -1, dont have to use malloc/ or check succession
***************************/
int josephus_array(int n) 
{
    int *soldiers = malloc(n * sizeof(int));
    
    for (int i = 0; i < n; ++i)
    {
       soldiers[i] = 1;
    } 

    int count = n, i = 0;
    
    while (count > 1) 
    {
        while (soldiers[i] == 0)
        { 
            i = (i + 1) % n;
        }
        
        int j = (i + 1) % n;
        
        while (soldiers[j] == 0)
        { 
            j = (j + 1) % n;
        }

        soldiers[j] = 0; 
        count--;
        i = (j + 1) % n; 
    }

    for (int i = 0; i < n; ++i)
        if (soldiers[i]) return i;

    free(soldiers);
    return -1;
}

int main() 
{
    int n = 100;
    int survivor = josephus_array(n);
    if (survivor >= 0)
        printf("The surviving soldier is at position: %d\n", survivor);
    else
        printf("Error occurred while computing survivor.\n");

    return 0;
}
