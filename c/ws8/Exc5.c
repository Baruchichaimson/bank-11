#include <stdio.h>
#include <stdint.h>


/***************************
 Exercise:  WS8
 Date: 	    17/06/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    
***************************/


union 
{
	float f;
 	uint32_t bits;
} float_union;

void PrintFloatBits(float n) 
{
	float_union.f = n;
	printf("%.2f\n", float_union.f);
    printf("Bits: ");
    for (int i = 31; i >= 0; i--) 
    {
        printf("%d", (float_union.bits >> i) & 1);
        if (i % 8 == 0) 
        {
        	printf(" ");
        }
    }
    printf("\n");

}

int main()
{
	PrintFloatBits(3.16f); 
}
