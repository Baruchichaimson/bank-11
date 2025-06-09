#include <stdio.h>
#include <stdlib.h>

/***************************
 Exercise:  WS4
 Date: 	    05/09/25
 Developer: Baruch Haimson
 Reviewer:  Daniel
 Status:    Approved
***************************/

void HandleKeySwitch(char c)
{
    switch (c)
    {
        case 'A': 
        	printf("A pressed\n");
                break;
        case 'T': 
                printf("T pressed\n"); 
                break;
        case 27:  
        	system("stty icanon echo");
        	exit(0);
    }
}

int main()
{
    char c;
    system("stty -icanon -echo");
    while (1)
    {    
        c = getchar();   
        HandleKeySwitch(c);
    }	

    return 0;
}
