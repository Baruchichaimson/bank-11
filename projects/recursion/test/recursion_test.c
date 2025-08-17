/************************************
 * Exercise: Recursion - Tests
 * Date: 17/08/25
 * Developer: Baruch Haimson
 * Reviewer: 
 * Status: In Progress
 ************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h> /* size_t */

#include "recursion.h"
#include "stack.h"

#define MAX_MEMO (1000)

int mem[MAX_MEMO];

static void TestFibonacci()
{
    printf("---- Test Fibonacci ----\n");
    printf("FibonacciRec(0) = %d (expected 0)\n", FibonacciRec(0));
    printf("FibonacciRec(1) = %d (expected 1)\n", FibonacciRec(1));
    printf("FibonacciRec(10) = %d (expected 55)\n", FibonacciRec(10));

    printf("FibonacciIter(0) = %d (expected 0)\n", FibonacciIter(0));
    printf("FibonacciIter(1) = %d (expected 1)\n", FibonacciIter(1));
    printf("FibonacciIter(10) = %d (expected 55)\n", FibonacciIter(10));

    /* FibonacciMemo test */
    printf("FibonacciMemo(0) = %d (expected 0)\n", FibonacciMemo(0));
    printf("FibonacciMemo(1) = %d (expected 1)\n", FibonacciMemo(1));
    printf("FibonacciMemo(10) = %d (expected 55)\n", FibonacciMemo(10));
}

static void TestStrFuncs()
{
    char buf[100];

    printf("---- Test String Functions ----\n");

    /* StrLen */
    printf("StrLen(\"hello\") = %lu (expected 5)\n", (unsigned long)StrLen("hello"));
    printf("StrLen(\"\") = %lu (expected 0)\n", (unsigned long)StrLen(""));

    /* StrCmp */
    printf("StrCmp(\"abc\",\"abc\") = %d (expected 0)\n", StrCmp("abc","abc"));
    printf("StrCmp(\"abc\",\"abd\") = %d (negative expected)\n", StrCmp("abc","abd"));
    printf("StrCmp(\"abd\",\"abc\") = %d (positive expected)\n", StrCmp("abd","abc"));
    printf("StrCmp(\"\",\"a\") = %d (negative expected)\n", StrCmp("","a"));
    printf("StrCmp(\"a\",\"\") = %d (positive expected)\n", StrCmp("a",""));

    /* StrCpy */
    StrCpy(buf, "copytest");
    printf("StrCpy -> %s (expected copytest)\n", buf);
    StrCpy(buf, "");
    printf("StrCpy empty -> \"%s\" (expected empty string)\n", buf);

    /* StrCat */
    StrCpy(buf, "hi ");
    StrCat(buf, "there");
    printf("StrCat -> %s (expected hi there)\n", buf);

    StrCpy(buf, "");
    StrCat(buf, "alone");
    printf("StrCat with empty dest -> %s (expected alone)\n", buf);

    /* StrStr */
    printf("StrStr(\"abcdef\",\"cde\") -> %s (expected cde...)\n", StrStr("abcdef","cde"));
    printf("StrStr(\"abcdef\",\"xyz\") -> %s (expected NULL)\n", StrStr("abcdef","xyz") ? "found" : "NULL");
    printf("StrStr(\"abcdef\",\"\") -> %s (expected full string)\n", StrStr("abcdef",""));
}

static void TestFlipList()
{
    node_t a;
    node_t b;
    node_t c;
    node_t* head;
    node_t* cur;

    int va = 1, vb = 2, vc = 3;

    printf("---- Test FlipList ----\n");

    a.data = &va;
    a.next = &b;
    b.data = &vb;
    b.next = &c;
    c.data = &vc;
    c.next = NULL;

    head = FlipList(&a);

    printf("FlipList result: ");
    for (cur = head; cur; cur = cur->next)
    {
        printf("%d ", *(int*)cur->data);
    }
    printf("(expected 3 2 1)\n");
}

static void TestSortStack()
{
    stack_t* st;
    int vals[] = {3,1,4,2};
    size_t i;

    printf("---- Test SortStack ----\n");

    st = StackCreate(10, sizeof(int));
    for (i = 0; i < 4; i++) 
    {
        StackPush(st, &vals[i]);
    }

    SortStack(st);

    printf("SortStack result (top->bottom): ");
    while (!StackIsEmpty(st))
    {
        printf("%d ", *(int*)StackPeek(st));
        StackPop(st);
    }
    printf("(expected 4 3 2 1)\n");

    StackDestroy(st);
}

static void InitMemo()
{
    size_t i = 0;

    for (i = 0; i < MAX_MEMO; ++i)
    {
        mem[i] = -1;
    }
}

int main()
{
    InitMemo();
    TestFibonacci();
    TestStrFuncs();
    TestFlipList();
    TestSortStack();

    return 0;
}
