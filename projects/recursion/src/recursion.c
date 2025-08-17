/************************************
 * Exercise: Recursion 
 * Date: 17/08/25
 * Developer: Baruch Haimson
 * Reviewer: 
 * Status: In Progress
 ************************************/

#include <string.h> /* strncmp*/

#include "recursion.h" /* API */
#include "stack.h"   /* StackPeek */

#define MAX_MEMO (1000)

int mem[MAX_MEMO];

int FibonacciRec(size_t n)
{
    if (n == 0) 
    {
        return 0; 
    } 
    if (n == 1) 
    {
        return 1; 
    }

    return FibonacciMemo(n - 1) + FibonacciMemo(n - 2);
}
int FibonacciMemo(size_t n)   
{
    if (n == 0) 
    {
        return 0; 
    } 
    if (n == 1) 
    {
        return 1; 
    }

    if (mem[n] != -1)
    {
        return mem[n];
    }

    mem[n] = FibonacciMemo(n - 1) + FibonacciMemo(n - 2);

    return mem[n];

    return -1;
}

int FibonacciIter(size_t n)
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

node_t* FlipList(node_t* node)
{
    node_t* new_head = NULL;

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
    if (*dest == '\0')  
    {
        StrCpy(dest, src); 
        return dest;
    }

    StrCat(dest + 1, src); 

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

static void InsertSorted(stack_t* s, int value)
{
    int temp = 0;

    if (StackIsEmpty(s) || value > *(int*)StackPeek(s))
    {
        StackPush(s, &value);
        return;
    }

    temp = *(int*)StackPeek(s);
    StackPop(s);
    InsertSorted(s, value);
    StackPush(s, &temp);
}

void SortStack(stack_t* s)
{
    int top = 0;

    if (!StackIsEmpty(s))
    {
        top = *(int*)StackPeek(s);

        StackPop(s);

        SortStack(s);

        InsertSorted(s, top);
    }
}