/***************************
 Exercise:  heap
 Date: 	    27/08/25
 Developer: Baruch Haimson
 Reviewer:  Tamar
 Status:    Approved
***************************/

#include <stdio.h>
#include <assert.h>

#include "heap.h"

/*============== helper functions =======================*/

static int CompareInt(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b); /* max heap */
}

static int IsMatchInt(const void *a, const void *b)
{
    return (*(int *)a == *(int *)b);
}

/*=========================================================*/

void TestHeapCreateDestroy()
{
    heap_t *heap = HeapCreate(CompareInt);
    assert(heap != NULL);
    assert(HeapIsEmpty(heap) == 1);
    assert(HeapSize(heap) == 0);
    HeapDestroy(heap);
    printf("TestHeapCreateDestroy: PASS\n");
}

void TestHeapPeek()
{
    heap_t *heap = HeapCreate(CompareInt);
    int x = 10;

    HeapPush(heap, &x);
    assert(*(int *)HeapPeek(heap) == 10);

    HeapDestroy(heap);
    printf("TestHeapPeek: PASS\n");
}

void TestHeapPush()
{
    heap_t *heap = HeapCreate(CompareInt);
    int a = 5, b = 20, c = 15;

    HeapPush(heap, &a);
    HeapPush(heap, &b);
    HeapPush(heap, &c);

    assert(HeapSize(heap) == 3);
    assert(*(int *)HeapPeek(heap) == 20); /* הכי גדול למעלה */

    HeapDestroy(heap);
    printf("TestHeapPush: PASS\n");
}

void TestHeapPop()
{
    heap_t *heap = HeapCreate(CompareInt);
    int a = 5, b = 20, c = 15;

    HeapPush(heap, &a);
    HeapPush(heap, &b);
    HeapPush(heap, &c);

    assert(*(int *)HeapPeek(heap) == 20);
    HeapPop(heap);
    assert(*(int *)HeapPeek(heap) == 15);

    HeapPop(heap);
    assert(*(int *)HeapPeek(heap) == 5);

    HeapPop(heap);
    assert(HeapIsEmpty(heap));

    HeapDestroy(heap);
    printf("TestHeapPop: PASS\n");
}

void TestHeapRemove()
{
    int a = 5, b = 20, c = 15, d = 7;
    int not_exist = 99;
    heap_t *heap = HeapCreate(CompareInt);
    
    HeapPush(heap, &a);
    HeapPush(heap, &b);
    HeapPush(heap, &c);
    HeapPush(heap, &d);

    assert(HeapSize(heap) == 4);

    HeapRemove(heap, &c, IsMatchInt);
    assert(HeapSize(heap) == 3);
    assert(*(int *)HeapPeek(heap) == 20);

    HeapRemove(heap, &not_exist, IsMatchInt);
    assert(HeapSize(heap) == 3);

    HeapRemove(heap, &b, IsMatchInt);
    assert(HeapSize(heap) == 2);
    assert(*(int *)HeapPeek(heap) != 20);

    HeapDestroy(heap);
    printf("TestHeapRemove: PASS\n");
}

void TestHeapIsEmpty()
{
    heap_t *heap = HeapCreate(CompareInt);
    int x = 1;

    assert(HeapIsEmpty(heap));
    HeapPush(heap, &x);
    assert(!HeapIsEmpty(heap));
    HeapPop(heap);
    assert(HeapIsEmpty(heap));

    HeapDestroy(heap);
    printf("TestHeapIsEmpty: PASS\n");
}

void TestHeapSize()
{
    heap_t *heap = HeapCreate(CompareInt);
    int x = 1, y = 2;

    assert(HeapSize(heap) == 0);
    HeapPush(heap, &x);
    HeapPush(heap, &y);
    assert(HeapSize(heap) == 2);
    HeapPop(heap);
    assert(HeapSize(heap) == 1);

    HeapDestroy(heap);
    printf("TestHeapSize: PASS\n");
}

int main()
{
    TestHeapCreateDestroy();
    TestHeapPeek();
    TestHeapPush();
    TestHeapPop();
    TestHeapRemove();
    TestHeapIsEmpty();
    TestHeapSize();

    printf("\nAll heap tests PASSED \n");

    return 0;
}
