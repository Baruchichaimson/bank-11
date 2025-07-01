#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

/***************************
 Exercise:  stack
 Date: 	    01/07/25
 Developer: Baruch Haimson
 Reviewer:  avi
 Status:    Approved
***************************/


void TestCreate()
{
    printf("=== Test: VectorCreate ===\n");

    vector_t* v1 = VectorCreate(5, sizeof(int));
    if (v1 && VectorCapacity(v1) == 8 && VectorSize(v1) == 0)
    {
        printf("\nCreate normal vector: PASS\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v1));
    }
    else
    {
        printf("\nCreate normal vector: FAIL\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v1));
    }
    VectorDestroy(v1);

    vector_t* v2 = VectorCreate(0, sizeof(int));
    if (v2 && VectorCapacity(v2) == 8)
    {
        printf("Create with 0 capacity: PASS\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v2));
    }
    else
    {
        printf("Create with 0 capacity: FAIL\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v2));
    }
    VectorDestroy(v2);

    vector_t* v3 = VectorCreate(10, 0);
    if (v3 != NULL)
    {
        printf("Create with 0 element size: PASS\n\tExpected: Not NULL\n\tActual: Not NULL\n\n");
    }
    else
    {
        printf("Create with 0 element size: FAIL\n\tExpected: Not NULL\n\tActual: NULL\n\n");
    }
    VectorDestroy(v3);
    
	vector_t* v4 = VectorCreate(16, sizeof(int));
    if (v4 && VectorCapacity(v4) == 16 && VectorSize(v4) == 0)
    {
        printf("Create normal vector: PASS\n\tExpected: 16\n\tActual: %zu\n\n", VectorCapacity(v4));
    }
    else
    {
        printf("Create normal vector: FAIL\n\tExpected: 16\n\tActual: %zu\n\n", VectorCapacity(v4));
    }
    VectorDestroy(v4);
}

void TestPushBack()
{
	size_t s = 0;
    printf("=== Test: VectorPushBack ===\n");

    vector_t* v = VectorCreate(1, sizeof(int));
    int val = 10;
    VectorPushBack(v, &val);
    if (*(int*)VectorGetAccessToElement(v, 0) == 10)
    {
        printf("\nPush normal value: PASS\n\tExpected: 10\n\tActual: %d\n", *(int*)VectorGetAccessToElement(v, 0));
        s = VectorCapacity(v);
        printf("\tThe capacity is:%lu\n\n", s);
    }
    else
    {
        printf("\nPush normal value: FAIL\n\tExpected: 10\n\tActual: %d\n\n", *(int*)VectorGetAccessToElement(v, 0));
    }

    int val2 = 20;
    VectorPushBack(v, &val2);
    if (*(int*)VectorGetAccessToElement(v, 1) == 20)
    {
        printf("Push beyond initial capacity: PASS\n\tExpected: 20\n\tActual: %d\n", *(int*)VectorGetAccessToElement(v, 1));
        s = VectorCapacity(v);
        printf("\tThe capacity is:%lu\n\n", s);
    }
    else
    {
        printf("Push beyond initial capacity: FAIL\n\tExpected: 20\n\tActual: %d\n\n", *(int*)VectorGetAccessToElement(v, 1));
    }

    int val3 = 30, val4 = 40, val5 = 50, val6 = 60, val7 = 70, val8 = 80, val9 = 90;
    VectorPushBack(v, &val3);
    VectorPushBack(v, &val4);
    VectorPushBack(v, &val5);
    VectorPushBack(v, &val6);
    VectorPushBack(v, &val7);
    VectorPushBack(v, &val8);
    VectorPushBack(v, &val9);
    if (*(int*)VectorGetAccessToElement(v, 2) == 30)
    {
        printf("Push another value: PASS\n\tExpected: 30\n\tActual: %d\n", *(int*)VectorGetAccessToElement(v, 2));
        s = VectorCapacity(v);
        printf("\tThe capacity is:%lu\n\n", s);
    }
    else
    {
        printf("Push another value: FAIL\n\tExpected: 30\n\tActual: %d\n\n", *(int*)VectorGetAccessToElement(v, 2));
    }
    VectorDestroy(v);
}

void TestPopBack()
{
	int i;
    int val100 = 100;
	size_t s = 0;
    printf("=== Test: VectorPopBack ===\n");

    vector_t* v = VectorCreate(4, sizeof(int));
    int val = 1, val2 = 2;
    VectorPushBack(v, &val);
    VectorPushBack(v, &val2);
    VectorPopBack(v);
    if (VectorSize(v) == 1)
    {
        printf("\nPop one element: PASS\n\tExpected: 1\n\tActual: %zu\n\n", VectorSize(v));
        s = VectorCapacity(v);
        printf("\tThe capacity is:%lu\n\n", s);
    }
    else
    {
        printf("\nPop one element: FAIL\n\tExpected: 1\n\tActual: %zu\n\n", VectorSize(v));
    }

    VectorPopBack(v);
    if (VectorSize(v) == 0)
    {
        printf("Pop to zero: PASS\n\tExpected: 0\n\tActual: %zu\n\n", VectorSize(v));
        s = VectorCapacity(v);
        printf("\tThe capacity is:%lu\n\n", s);
    }
    else
    {
        printf("Pop to zero: FAIL\n\tExpected: 0\n\tActual: %zu\n\n", VectorSize(v));
    }

    VectorPushBack(v, &val);
    VectorPushBack(v, &val2);
    VectorPushBack(v, &val2);
    VectorPushBack(v, &val2);
    VectorPushBack(v, &val2); 
    VectorPopBack(v);         
    if (VectorSize(v) == 4)
    {
        printf("Pop with shrink: PASS\n\tExpected: 4\n\tActual: %zu\n\n", VectorSize(v));
        s = VectorCapacity(v);
        printf("\tThe capacity is:%lu\n\n", s);
    }
    else
    {
        printf("Pop with shrink: FAIL\n\tExpected: 4\n\tActual: %zu\n\n", VectorSize(v));
    }

    VectorDestroy(v);

	vector_t* v2 = VectorCreate(20, sizeof(int));
    
    for (i = 0; i < 6; ++i)
    {
        VectorPushBack(v2, &val100);
    }
    s = VectorCapacity(v);
    printf("\tThe capacity before pop is:%lu\n\n", s);
    VectorPopBack(v2);
	if (VectorSize(v) == 5)
    {
        printf("Pop to zero: PASS\n\tExpected: 5\n\tActual: %zu\n\n", VectorSize(v));
        s = VectorCapacity(v);
        printf("\tThe capacity after pop is:%lu\n\n", s);
    }
    else
    {
        printf("Pop to zero: FAIL\n\tExpected: 6\n\tActual: %zu\n\n", VectorSize(v));
        s = VectorCapacity(v);
        printf("\tThe capacity is:%lu\n\n", s);
    } 

    VectorDestroy(v);
}

void TestReserve()
{
    printf("=== Test: VectorReserve ===\n");

    vector_t* v = VectorCreate(2, sizeof(int));
    VectorReserve(v, 10);
    if (VectorCapacity(v) == 10)
    {
        printf("\nReserve bigger capacity: PASS\n\tExpected: 10\n\tActual: %zu\n\n", VectorCapacity(v));
    }
    else
    {
        printf("\nReserve bigger capacity: FAIL\n\tExpected: 10\n\tActual: %zu\n\n", VectorCapacity(v));
    }

    VectorPushBack(v, &(int){1});
    VectorPushBack(v, &(int){2});
    VectorReserve(v, 1); 
    if (VectorCapacity(v) == 8)
    {
        printf("Reserve smaller than size: PASS\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v));
    }
    else
    {
        printf("Reserve smaller than size: FAIL\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v));
    }

    VectorDestroy(v);
}

void TestShrink()
{
    printf("=== Test: VectorShrink ===\n");

    vector_t* v = VectorCreate(10, sizeof(int));
    VectorPushBack(v, &(int){1});
    VectorPushBack(v, &(int){2});
    VectorShrink(v);
    if (VectorCapacity(v) == 8)
    {
        printf("\nShrink to size: PASS\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v));
    }
    else
    {
        printf("\nShrink to size: FAIL\n\tExpected: 8\n\tActual: %zu\n\n", VectorCapacity(v));
    }

    VectorDestroy(v);
    
    vector_t* v2 = VectorCreate(10, sizeof(int));
    VectorPushBack(v2, &(int){1});
    VectorPushBack(v2, &(int){2});
    VectorPushBack(v2, &(int){3});
    VectorPushBack(v2, &(int){4});
    VectorPushBack(v2, &(int){5});
    VectorPushBack(v2, &(int){6});
    VectorPushBack(v2, &(int){7});
    VectorPushBack(v2, &(int){8});
    VectorPushBack(v2, &(int){9});
    VectorShrink(v2);
    if (VectorCapacity(v2) == 9)
    {
        printf("\nShrink to size: PASS\n\tExpected: 9\n\tActual: %zu\n\n", VectorCapacity(v2));
    }
    else
    {
        printf("\nShrink to size: FAIL\n\tExpected: 9\n\tActual: %zu\n\n", VectorCapacity(v2));
    }

    VectorDestroy(v2);
}

int main()
{
    TestCreate();
    TestPushBack();
    TestPopBack();
    TestReserve();
    TestShrink();
    return 0;
}
