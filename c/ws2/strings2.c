#include <stdio.h>
#include "strings2.h"
#include <assert.h>

int StrCmp (char string1[], char string2[])
{
    for (int i = 0;; i++)
    {
        if (string1[i] != string2[i])
        {
            return string1[i] < string2[i] ? -1 : 1;
        }

        if (string1[i] == '\0')
        {
            return 0;
        }
    }
}

int main()
{
    char* s1 = "baruch haimson";
    char* s2 = "baruch haimson";
    char* s3 = "baruch iaimson";
    char* s4 = "baruch aimson";

    assert(StrCmp(s1, s2) == 0);
    assert(StrCmp(s1, s3) == -1); 
    assert(StrCmp(s1, s4) == 1);   

    printf("All StrCmp tests passed successfully!\n");

    return 0;
}
