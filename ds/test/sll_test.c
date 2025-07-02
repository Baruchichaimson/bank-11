#include <stdio.h>
#include <stdlib.h>
#include "sll.h"

int MatchInt(const void* data, void* param);

int MultiplyByTwo(void* data, void* param);

int FailOnThree(void* data, void* param);

int FreeData(void* data, void* param);


void TestSLLCreateAndIsEmpty()
{
    printf("TestSLLCreateAndIsEmpty:\n");

    sll_t* list = SLLCreate();
    if (list != NULL)
    {
        printf("PASS: list created (Expected: not NULL, Actual: not NULL)\n");
    }
    else
    {
        printf("FAIL: list not created (Expected: not NULL, Actual: NULL)\n");
        return;
    }

    if (SLLIsEmpty(list))
    {
        printf("PASS: list is empty (Expected: 1, Actual: 1)\n");
    }
    else
    {
        printf("FAIL: list not empty (Expected: 1, Actual: 0)\n");
    }

    SLLDestroy(list);
    printf("\n");
}

void TestSLLInsertAndCount()
{
	size_t count = 0;
    printf("TestSLLInsertAndCount:\n");
	
    sll_t* list = SLLCreate();
    node_t* currentNode = SLLEnd(list);

    int* a = malloc(sizeof(int)); *a = 10;
    int* b = malloc(sizeof(int)); *b = 20;
    int* c = malloc(sizeof(int)); *c = 30;

    SLLInsert(SLLEnd(list), c);
    SLLInsert(SLLEnd(list), b);
    SLLInsert(SLLEnd(list), a);

    count = SLLCount(list);
    if (count == 3)
    {
        printf("PASS: count correct (Expected: 3, Actual: 3)\n");
        int valhead = *(int*)SLLGetData(SLLBegin(list));
        while(SLLNext(currentNode) != SLLEnd(list))
		{
			currentNode = SLLNext(currentNode);
		}
       
        int valtail = *(int*)SLLGetData(currentNode);
      
        printf("\thead : %d , tail: %d\n", valhead, valtail);
        
    }
    else
    {
        printf("FAIL: count incorrect (Expected: 3, Actual: %lu)\n", count);
    }

    SLLDestroy(list);
    printf("\n");
}

void TestSLLGetSetData()
{
	int new_val = 42;
	int actual;
    printf("TestSLLGetSetData:\n");

    sll_t* list = SLLCreate();
    int* val = malloc(sizeof(int));
    *val = 100;
    SLLInsert(SLLEnd(list), val);

    SLLSetData(SLLBegin(list), &new_val);
    
    actual = *(int*)SLLGetData(SLLBegin(list));

    if (actual == 42)
    {
        printf("PASS: Set/Get Data works (Expected: 42, Actual: 42)\n");
    }
    else
    {
        printf("FAIL: Set/Get Data failed (Expected: 42, Actual: %d)\n", actual);
    }

    SLLDestroy(list);
    printf("\n");
}

void TestSLLIsEqualAndNext()
{
    printf("TestSLLIsEqualAndNext:\n");

    sll_t* list = SLLCreate();
    int* a = malloc(sizeof(int)); *a = 5;
    int* b = malloc(sizeof(int)); *b = 6;

    sll_iter_t iter1 = SLLInsert(SLLEnd(list), b);
    sll_iter_t iter2 = SLLInsert(SLLEnd(list), a);

    if (!SLLIsEqual(iter1, iter2))
    {
        printf("PASS: iter1 != iter2\n");
    }
    else
    {
        printf("FAIL: iter1 == iter2 (Expected: different)\n");
    }

    if (SLLIsEqual(SLLNext(iter1), iter2))
    {
        printf("PASS: SLLNext works (iter1->next == iter2)\n");
    }
    else
    {
        printf("FAIL: SLLNext failed (iter1->next != iter2)\n");
    }

    free(a);
    free(b);
    SLLDestroy(list);
    printf("\n");
}

void TestSLLFind()
{
    printf("TestSLLFind:\n");

    sll_t* list = SLLCreate();
    int* x = malloc(sizeof(int)); *x = 11;
    int* y = malloc(sizeof(int)); *y = 22;
    SLLInsert(SLLEnd(list), y);
    SLLInsert(SLLEnd(list), x);

    int target = 22;
    sll_iter_t found = SLLFind(SLLBegin(list), SLLEnd(list), MatchInt, &target);

    int val = *(int*)SLLGetData(found);
    if (val == 22)
    {
        printf("PASS: SLLFind found 22\n");
    }
    else
    {
        printf("FAIL: SLLFind did not find 22 (Got: %d)\n", val);
    }

    target = 99;
    found = SLLFind(SLLBegin(list), SLLEnd(list), MatchInt, &target); 
    if (SLLIsEqual(found, SLLEnd(list)))
    {
        printf("PASS: SLLFind did not find 99, as expected\n");
    }
    else
    {
        printf("FAIL: SLLFind should return end when not found\n");
    }

    free(x);
    free(y);
    SLLDestroy(list);
    printf("\n");
}


void TestSLLForEach()
{
    printf("TestSLLForEach:\n");

    sll_t* list = SLLCreate();
    int* a = malloc(sizeof(int)); *a = 1;
    int* b = malloc(sizeof(int)); *b = 2;
    int* c = malloc(sizeof(int)); *c = 3;

    SLLInsert(SLLEnd(list), c);
    SLLInsert(SLLEnd(list), b);
    SLLInsert(SLLEnd(list), a);
	
	int result = SLLForEach(SLLBegin(list), SLLEnd(list), MultiplyByTwo, NULL);
    if (result == 0 && *(int*)SLLGetData(SLLBegin(list)) == 6)
    {
        printf("PASS: ForEach MultiplyByTwo succeeded\n");
    }
    else
    {
        printf("FAIL: ForEach MultiplyByTwo failed\n");
    }
	
    result = SLLForEach(SLLBegin(list), SLLEnd(list), FailOnThree, NULL);
    if (result == 1)
    {
        printf("PASS: ForEach stopped on value 6\n");
    }
    else
    {
        printf("FAIL: ForEach did not stop on 6 as expected\n");
    }

	SLLForEach(SLLBegin(list), SLLEnd(list), FreeData, NULL);
    SLLDestroy(list);
    printf("\n");
}


void TestSLLRemove()
{
    printf("TestSLLRemove:\n");

    sll_t* list = SLLCreate();
    int* a = malloc(sizeof(int)); *a = 77;
    int* b = malloc(sizeof(int)); *b = 88;

    SLLInsert(SLLEnd(list), b);
    SLLInsert(SLLEnd(list), a);

    size_t before = SLLCount(list);
    SLLRemove(SLLBegin(list));
    size_t after = SLLCount(list);

    if (after == before - 1)
    {
        printf("PASS: Remove reduced count (Expected: %lu, Actual: %lu)\n", before - 1, after);
    }
    else
    {
        printf("FAIL: Remove count mismatch (Expected: %lu, Actual: %lu)\n", before - 1, after);
    }

    free(a);
    free(b);
    SLLDestroy(list);
    printf("\n");
}
/* =============================match and action functions =====================================*/
int MatchInt(const void* data, void* param)
{
    return *(int*)data == *(int*)param;
}

int MultiplyByTwo(void* data, void* param)
{
    (void)param;
    *(int*)data *= 2;
    return 1;
}


int FailOnThree(void* data, void* param)
{
    (void)param;
    return (*(int*)data != 6);
}

int FreeData(void* data, void* param)
{
    (void)param;
    free(data);
    return 1; 
}

/* ===============================================================================================*/

int main()
{
    TestSLLCreateAndIsEmpty();
    TestSLLInsertAndCount();
    TestSLLGetSetData();
    TestSLLIsEqualAndNext();
    TestSLLFind();
    TestSLLForEach();
    TestSLLRemove();

    return 0;
}

