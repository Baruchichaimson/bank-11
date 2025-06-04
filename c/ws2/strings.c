#include "strings.h"
#include <stdlib.h>
#include <assert.h>
/***************************
author:Baruch Haimson
reviewer:
status:approved

***************************/
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

int StrCmp(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
    }
    return (*s1 < *s2) ? -1 : (*s1 > *s2) ? 1 : 0;
}

char* StrCpy(char *dst, const char *src)
{
    char *temp = dst;
    assert(dst != NULL && src != NULL);
    while ((*dst++ = *src++) != '\0');
    return temp;
}

char* StrnCpy(char *dst, const char *src, size_t n)
{
    char* start = dst;
    size_t count = 0;
    assert(dst != NULL && src != NULL);  

    while(count < n && *src != '\0') 
    {
        *dst++ = *src++;
        count++;
    }
    *dst = '\0';

    return start;
}

int StrnCmp(const char* s1, const char* s2, size_t n)
{
    if (n == 0) return 0;
    while (*s1 && (*s1 == *s2) && (--n))
    {
        s1++;
        s2++;
    }
    if (n == 0) return 0;
    return (*s1 < *s2) ? -1 : (*s1 > *s2) ? 1 : 0;
}

int StrCaseCmp(const char *s1, const char *s2)
{
    unsigned char c1, c2;
    while (*s1 && *s2) 
    {
        c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
        if (c1 != c2)
            return (c1 < c2) ? -1 : 1;
        s1++;
        s2++;
    }
    c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
    c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;

    return (c1 < c2) ? -1 : (c1 > c2) ? 1 : 0;
}

char* StrChr(const char *str, int c)
{
    while (*str != '\0')
    {
        if (*str == (char)c)
        {
            return (char *)str;
        }
        str++;
    }
    if ((char)c == '\0') 
    {
        return (char *)str;
    }
    return NULL;
}

char* StrDup(const char *src)
{
    char *str;
    char *p;
    int len = 0;

    while (src[len])
        len++;
    str = malloc(len + 1);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}

char* StrCat(char *dest, const char *src)
{
    char *p = dest;
    while (*p != '\0') 
    {
        p++;
    }
    while (*src != '\0') 
    {
        *p = *src;
        p++;
        src++;
    }
    *p = '\0';
    return dest;
}

char* StrnCat(char *dest, const char *src, size_t n) 
{
    char *p = dest;
    while (*p != '\0') 
    {
        p++;
    }
    while (*src != '\0' && n > 0) 
    {
        *p = *src;
        p++;
        src++;
        n--;
    }
    *p = '\0';

    return dest;
}

char* StrStr(const char *haystack, const char *needle) 
{
    const char *h, *n;
    
    if (*needle == '\0') 
    {
        return (char *)haystack; 
    }

    while (*haystack) 
    {
        h = haystack;
        n = needle;

        while (*h && *n && *h == *n) 
        {
            h++;
            n++;
        }

        if (*n == '\0') 
        {
            return (char *)haystack; 
        }

        haystack++;
    }

    return NULL; 
}

int is_in(const char *accept, char c) 
{
    while (*accept) 
    {
        if (*accept == c) 
        {
            return 1;  
        }
        accept++;
    }
    return 0;  
}

size_t StrSpn(const char *s, const char *accept) 
{
    size_t count = 0;

    while (*s) 
    {
        if (!is_in(accept, *s)) 
        {
            break;  
        }
        count++;
        s++;
    }

    return count;
}

char* StrTok(char *str, const char *delim) 
{
    static char *next = NULL;
    char *start;
    
    if (str) 
    {
        next = str;     
    }

    if (next == NULL) 
    {
        return NULL;    
    }

    /* Skip leading delimiters */
    while (*next && is_in(delim, *next)) 
    {
        next++;
    }

    if (*next == '\0') 
    {
        return NULL;    
    }

    start = next;

    /* Find end of token */
    while (*next && !is_in(delim, *next)) 
    {
        next++;
    }

    if (*next) 
    {
        *next = '\0';   
        next++;        
    } 
    else 
    {
        next = NULL;    
    }

    return start;
}
