#include <stdio.h>

/***************************
 Exercise:  WS8
 Date: 	    17/06/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    
***************************/


#define SIZEOF_TYPE(type) ((char*)(((type*)0) + 1) - (char*)((type*)0))
/* (type*)0 => to show as what is the size of the type from address 0 */

struct MyStruct 
{
    int id;
    double value;
    char c;
};

int main() 
{
    printf("SIZEOF_TYPE(int) = %zu\n", SIZEOF_TYPE(int));
    printf("SIZEOF_TYPE(double) = %zu\n", SIZEOF_TYPE(double)); 
    printf("SIZEOF_TYPE(struct MyStruct) = %zu\n", SIZEOF_TYPE(struct MyStruct)); 
    /* affter padding => 24 */

    return 0;
}
