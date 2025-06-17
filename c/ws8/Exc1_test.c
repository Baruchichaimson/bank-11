#include <stdio.h>
#include "Exc1.h"

/***************************
 Exercise:  WS8
 Date: 	    17/06/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    
***************************/

int main()
{
    Operator element[6];
    size_t size_ops;

    InitInt(&element[0], 7);
    InitInt(&element[1], 15);
    InitFloat(&element[2], 3.14f);
    InitFloat(&element[3], 2.71f);
    InitStr(&element[4], "haimson");
    InitStr(&element[5], "baruchi");

    size_ops = sizeof(element) / sizeof(element[0]);

    printf("=== Before ADD ===\n");
    printAll(element, size_ops);

    printf("\n=== After ADD ===\n");
    addAll(element, size_ops);
    printAll(element, size_ops);

    printf("\n=== After CLEAR ===\n");
    clearAll(element, size_ops);
    printAll(element, size_ops);

	freeAll(element, size_ops);
    return 0;
}

