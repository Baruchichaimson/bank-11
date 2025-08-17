/************************************
 * Exercise: Recursion 
 * Date: 17/08/25
 * Developer: Baruch Haimson
 * Reviewer: 
 * Status: In Progress
 ************************************/

#include <string.h>  /* StrStr */
#include "recursion.h"

int FibonacciRec(int n)
{
    if (n == 0) 
    {
        return 0; 
    } 
    if (n == 1) 
    {
        return 1; 
    }

    return FibonacciRec(n - 1) + FibonacciRec(n - 2);
}

int FibonacciIter(int n)
{
    int prevprev = 0;
    int prev = 1; 
    int current = 0;
    size_t i = 0;

    if (n == 0) 
    {
        return 0; 
    } 
    if (n == 1) 
    {
        return 1; 
    }

    for (i = 2; i <= n; i++)
    {
        current = prevprev + prev;
        prevprev = prev;
        prev = current;
    }
    return current;
}

Node* FlipList(Node* node)
{
    Node* new_head = NULL;

    if (!node || !node->next) 
    { 
        return node;
    }

    new_head = FlipList(node->next);

    node->next->next = node;   
    node->next = NULL;  

    return new_head;
}


size_t StrLen(const char* str)
{
    if (*str == '\0') 
    {
        return 0;
    }
    return 1 + StrLen(str + 1);
}

int StrCmp(const char* s1, const char* s2)
{
    if (*s1 == '\0' && *s2 == '\0') 
    {
        return 0;
    }
    if (*s1 != *s2) 
    {
        return (unsigned char)*s1 - (unsigned char)*s2;
    }
    return StrCmp(s1 + 1, s2 + 1);
}

char* StrCpy(char* dest, const char* src)
{
    if (*src == '\0') 
    { 
        *dest = '\0'; 
        return dest; 
    }

    *dest = *src;
    StrCpy(dest + 1, src + 1);

    return dest;
}

char* StrCat(char* dest, const char* src)
{
    while (*dest) 
    {
        dest++;
    }

    StrCpy(dest, src);

    return dest;
}

char* StrStr(const char* haystack, const char* needle)
{
    if (!*needle) 
    {
        return (char*)haystack; 
    }
    if (!*haystack) 
    {
        return NULL;
    }

    if (strncmp(haystack, needle, StrLen(needle)) == 0)
    {
        return (char*)haystack;
    }

    return StrStr(haystack + 1, needle);
}
