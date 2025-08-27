/***************************
 * Exercise: PQ Heap
 * Date:     27/08/25
 * Developer: Baruch Haimson
 * Reviewer: tamar
 * Status:   Approved
 ***************************/

#include <stdio.h>
#include <stdlib.h>

#include "pq_heap.h"

/*================= helper functions ============================*/

int CompareInts(const void *data1, const void *data2);
int IsMatch(const void *data, const void *param);

/*==============================================================*/

void TestCreateDestroy()
{
    pq_heap_t *pq;

    printf("\n=== TestCreateDestroy ===\n");

    pq = PQHeapCreate(CompareInts);
    printf("PQHeapCreate: %s\n", (pq != NULL) ? "PASS" : "FAIL");

    PQHeapDestroy(pq);
    printf("PQHeapDestroy: PASS (no crash)\n");
}

void TestEnqueuePeek()
{
    pq_heap_t *pq;
    int a = 5, b = 1, c = 10;

    printf("\n=== TestEnqueuePeek ===\n");

    pq = PQHeapCreate(CompareInts);

    PQHeapEnqueue(pq, &a);
    printf("Peek (Expect 5 highest): %d\n", *(int *)PQHeapPeek(pq));

    PQHeapEnqueue(pq, &b);
    printf("Peek after insert 1: %d (Expected 5)\n", *(int *)PQHeapPeek(pq));

    PQHeapEnqueue(pq, &c);
    printf("Peek after insert 10: %d (Expected 10)\n", *(int *)PQHeapPeek(pq));

    PQHeapDestroy(pq);
}

void TestDequeue()
{
    pq_heap_t *pq;
    int a = 3, b = 7, c = 1;
    void *data;

    printf("\n=== TestDequeue ===\n");

    pq = PQHeapCreate(CompareInts);

    if (PQHeapIsEmpty(pq))
    {
        printf("Dequeue empty: PASS\n");
    }
    else
    {
        printf("Dequeue empty: FAIL\n");
    }

    PQHeapEnqueue(pq, &a);
    PQHeapEnqueue(pq, &b);
    PQHeapEnqueue(pq, &c);

    data = PQHeapDequeue(pq);
    printf("Dequeue first: %d (Expected 7)\n", *(int *)data);

    data = PQHeapDequeue(pq);
    printf("Dequeue second: %d (Expected 3)\n", *(int *)data); 

    data = PQHeapDequeue(pq);
    printf("Dequeue third: %d (Expected 1)\n", *(int *)data);

    if (PQHeapIsEmpty(pq))
    {
        printf("Dequeue after empty: PASS\n");
    }
    else
    {
        printf("Dequeue after empty: FAIL\n");
    }

    PQHeapDestroy(pq);
}

void TestIsEmptyAndSize()
{
    pq_heap_t *pq;
    int a = 4, b = 2;

    printf("\n=== TestIsEmptyAndSize ===\n");

    pq = PQHeapCreate(CompareInts);

    printf("Empty on create: %d (Expected 1)\n", PQHeapIsEmpty(pq));
    printf("Size empty: %lu (Expected 0)\n", PQHeapSize(pq));

    PQHeapEnqueue(pq, &a);
    PQHeapEnqueue(pq, &b);

    printf("IsEmpty after enqueue: %d (Expected 0)\n", PQHeapIsEmpty(pq));
    printf("Size after enqueue: %lu (Expected 2)\n", PQHeapSize(pq));

    PQHeapDequeue(pq);
    printf("Size after dequeue: %lu (Expected 1)\n", PQHeapSize(pq));

    PQHeapDequeue(pq);
    printf("Empty after dequeue all: %d (Expected 1)\n", PQHeapIsEmpty(pq));

    PQHeapDestroy(pq);
}

void TestClear()
{
    pq_heap_t *pq;
    int a = 10, b = 20, c = 30;

    printf("\n=== TestClear ===\n");

    pq = PQHeapCreate(CompareInts);

    PQHeapEnqueue(pq, &a);
    PQHeapEnqueue(pq, &b);
    PQHeapEnqueue(pq, &c);

    printf("Size before clear: %lu (Expected 3)\n", PQHeapSize(pq));
    PQHeapClear(pq);
    printf("Size after clear: %lu (Expected 0)\n", PQHeapSize(pq));
    printf("IsEmpty after clear: %d (Expected 1)\n", PQHeapIsEmpty(pq));

    PQHeapDestroy(pq);
}

void TestErase()
{
    pq_heap_t *pq;
    int a = 4, b = 8, c = 2, erase_val = 4;

    printf("\n=== TestErase ===\n");

    pq = PQHeapCreate(CompareInts);

    PQHeapEnqueue(pq, &a);
    PQHeapEnqueue(pq, &b);
    PQHeapEnqueue(pq, &c);

    printf("Size before erase: %lu (Expected 3)\n", PQHeapSize(pq));

    PQHeapErase(pq, IsMatch, &erase_val);
    printf("Size after erase 4: %lu (Expected 2)\n", PQHeapSize(pq));

    printf("Peek after erase: %d (Expected 8)\n", *(int *)PQHeapPeek(pq));

    PQHeapClear(pq);
    PQHeapErase(pq, IsMatch, &erase_val);
    printf("Erase on empty queue: PASS (no crash)\n");

    PQHeapDestroy(pq);
}

int main()
{
    TestCreateDestroy();
    TestEnqueuePeek();
    TestDequeue();
    TestIsEmptyAndSize();
    TestClear();
    TestErase();

    printf("\nAll PQHeap tests completed.\n");
    return 0;
}

/*================= helper functions ============================*/

int CompareInts(const void *data1, const void *data2)
{
    int num1 = *(const int *)data1;
    int num2 = *(const int *)data2;
    return num1 - num2; 
}

int IsMatch(const void *data, const void *param)
{
    return (*(int *)data == *(int *)param);
}
