#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int IsStringRotation(const char* str1, const char* str2)
{
    /* ### Write your code below this line ### */
    int match;
    int i = 0, j = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    
    if (len2 == 0)
    {
        return -1;
    }
    if (len2 > 2 * len1)
    {
        return -1;
    }   

    for (i = 0; i < 2 * len1 - len2 + 1; ++i) 
    {
        match = 1;
        for (j = 0; j < len2; ++j) 
        {
            if (str1[(i + j) % len1] != str2[j]) 
            {
                match = 0;
                break;
            }
        }
        if (match) 
        {
            return i;
        }
    }
    return -1;
}
