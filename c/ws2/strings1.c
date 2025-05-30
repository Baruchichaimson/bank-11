#include <stdio.h>
#include "strings1.h"

size_t StrLen (const char* str)
{
    if (str == NULL)
    {
        return 0;
    }
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}

int main()
{
    const char* Nullable = NULL;
    printf("the length of the string is: %zu \n", StrLen("baruch haimson"));
    printf("the length of the string is: %zu \n", StrLen(""));
    size_t len = StrLen(Nullable);
    if (len == 0 && Nullable == NULL)
        printf("The string is not good!\n");
    else
        printf("the length of the string is: %zu \n", len);
    return 0;
}

