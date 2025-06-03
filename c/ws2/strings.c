#include "strings.h"

size_t StrLen(const char *str)
{
    const char *ptr;
    if (str == NULL)
    {
        return 0;
    }
    ptr = str;
    while (*ptr)
    {
        ptr++;
    }
    return ptr - str;
}

int StrCmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*s1 < *s2) ? -1 : (*s1 > *s2) ? 1 : 0;
}


