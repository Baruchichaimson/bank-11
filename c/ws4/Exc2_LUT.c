#include <stdio.h>
#include <stdlib.h>

/***************************
 Exercise:  WS4
 Date: 	    05/09/25
 Developer: Baruch Haimson
 Reviewer:  Daniel
 Status:    Approved
***************************/
void pressA() { printf("A-pressed\n"); }
void pressT() { printf("T-pressed\n"); }
void empty() {}
void pressEsc() 
{
 	system("stty icanon echo");
        exit(0);
}
void HandleKeyLUT(char c)
{
    typedef void (*keyDo)();
    keyDo arrTav[256];
    
    for (int i = 0; i < 256; ++i)
    {
            arrTav[i] = empty;
    }
    arrTav['A'] = pressA;
    arrTav['T'] = pressT;
    arrTav[27] = pressEsc;
    
    arrTav[(unsigned char)c]();
    
}

int main()
{
    char c;
    system("stty -icanon -echo");
    while (1)
    {    
        c = getchar();   
        HandleKeyLUT(c);
    }	

    return 0;
}
