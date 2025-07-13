/***************************
 * Exercise: Sorted List
 * Date:     10/07/25
 * Developer: Baruch Haimson
 * Reviewer: 
 * Status:   In Progress
 ***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sortedl.h"
#include "dll.h"

typedef struct person
{
    int id;
    char name[100];
} person;

static int CompareInts(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

static int ActionPrint(void* data, void* param)
{
    (void)param;
    printf("%d ", *(int*)data);
    return 1;
}

void TestCreateDestroy()
{
    printf("\n-- TestCreateDestroy --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    printf("List created: %s\n", list ? "PASS" : "FAIL");
    SortedLDestroy(list);
    printf("List destroyed.\n");
}

void TestInsert()
{
    printf("\n-- TestInsert --\n");

    sortedl_t* list = SortedLCreate(CompareInts);

    int a = 10;
    int b = 20;
    int c = 9;
    int d = 15;

    SortedLInsert(list, &a);
    SortedLInsert(list, &b);
    SortedLInsert(list, &c);
    SortedLInsert(list, &d);

    printf("List contents:\n");
    SortedLForEach(SortedLBegin(list), SortedLEnd(list), ActionPrint, NULL);
    printf("\nExpected: 9 10 15 20\n");

    SortedLDestroy(list);
}



void TestRemove()
{
	sorted_iter_t it;
    printf("\n-- TestRemove --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    
    int a = 5;
    int b = 10;
    int c = 50;
    int d = 20;
    
    SortedLInsert(list, &a);
    SortedLInsert(list, &b);
    it = SortedLInsert(list, &c);
	SortedLInsert(list, &d);
	    
    SortedLRemove(it);
    SortedLRemove(SortedLPrev(it));
    
    SortedLForEach(SortedLBegin(list), SortedLEnd(list), ActionPrint, NULL);
    
    printf("\nExpected: 5 10 \n");
    
    SortedLDestroy(list);
}

void TestSizeIsEmpty()
{
    printf("\n-- TestSizeIsEmpty --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    printf("IsEmpty: %d\n", SortedLIsEmpty(list));
    int x = 1;
    SortedLInsert(list, &x);
    printf("Size: %lu\n", SortedLSize(list));
    SortedLDestroy(list);
}

void TestFind()
{
    int to_find = 2;
    
    sorted_iter_t res1;
    
    sortedl_t* list = SortedLCreate(CompareInts);

    printf("\n-- TestFind --\n");

    int arr[] = {1, 2, 3};
    for (int i = 0; i < 3; ++i)
    {
        SortedLInsert(list, &arr[i]);
    }

    res1 = SortedLFind(list, SortedLBegin(list), SortedLEnd(list), &to_find);
    if (!SortedLIsEqual(res1, SortedLEnd(list)))
    {
        printf("Found: %d\n", *(int*)SortedLGetData(res1));
    }
    else
    {
        printf("Not found, returned end()\n");
    }
    printf("Expected: 2\n");

    SortedLDestroy(list);
}


int MatchInt(const void* data, const void* param)
{
    return *(int*)data == *(int*)param;
}

int CompareByName(const void* a, const void* b)
{
    const person* p1 = (const person*)a;
    const person* p2 = (const person*)b;
    return strcmp(p1->name, p2->name);
}

void TestFindIf()
{
    printf("\n-- TestFindIf --\n");
    
    sortedl_t* list = SortedLCreate(CompareInts);
    
    int val = 6;
    int arr[] = {3, 6, 9};
    
    for (int i = 0; i < 3; ++i) 
    { 
    	SortedLInsert(list, &arr[i]); 
    }
   
    sorted_iter_t res = SortedLFindIf(SortedLBegin(list), SortedLEnd(list), MatchInt, &val);
    
    printf("FoundIf: %d\n", *(int*)SortedLGetData(res));
    printf("Expected: 6\n");
    
    SortedLDestroy(list);
}

void TestIsEqual()
{
    printf("\n-- TestIsEqual --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    int a = 4;
    sorted_iter_t it1 = SortedLInsert(list, &a);
    sorted_iter_t it2 = it1;
    printf("IsEqual: %d\n", SortedLIsEqual(it1, it2));
    SortedLDestroy(list);
}

void TestNextPrev()
{
    printf("\n-- TestNextPrev --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    int a = 1, b = 2;
    sorted_iter_t it1 = SortedLInsert(list, &a);
    sorted_iter_t it2 = SortedLInsert(list, &b);
    sorted_iter_t next = SortedLNext(it1);
    sorted_iter_t prev = SortedLPrev(it2);
    printf("Next of 1: %d\n", *(int*)SortedLGetData(next));
    printf("Prev of 2: %d\n", *(int*)SortedLGetData(prev));
    SortedLDestroy(list);
}

void TestBeginEnd()
{
    printf("\n-- TestBeginEnd --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    int a = 8;
    SortedLInsert(list, &a);
    printf("Begin: %d\n", *(int*)SortedLGetData(SortedLBegin(list)));
    SortedLDestroy(list);
}

void TestPopFrontBack()
{
    printf("\n-- TestPopFrontBack --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    int a = 3, b = 9;
    SortedLInsert(list, &a);
    SortedLInsert(list, &b);
    printf("PopFront: %d\n", *(int*)SortedLPopFront(list));
    printf("PopBack: %d\n", *(int*)SortedLPopBack(list));
    SortedLDestroy(list);
}

void TestMerge()
{
    printf("\n-- TestMerge --\n");
    sortedl_t* l1 = SortedLCreate(CompareInts);
    sortedl_t* l2 = SortedLCreate(CompareInts);
    int a[] = {1, 4, 6};
    int b[] = {2, 3, 5};
    for (int i = 0; i < 3; ++i) 
    { 
    	SortedLInsert(l1, &a[i]); 
    	SortedLInsert(l2, &b[i]); 
    }
    
    SortedLMerge(l1, l2);
    
    SortedLForEach(SortedLBegin(l1), SortedLEnd(l1), ActionPrint, NULL);
    
    printf("\nExpected: 1 2 3 4 5 6\n");
    
    SortedLDestroy(l1);
    SortedLDestroy(l2);
}

void TestForEach()
{
    printf("\n-- TestForEach --\n");
    sortedl_t* list = SortedLCreate(CompareInts);
    int arr[] = {7, 2, 5};
    for (int i = 0; i < 3; ++i) { SortedLInsert(list, &arr[i]); }
    SortedLForEach(SortedLBegin(list), SortedLEnd(list), ActionPrint, NULL);
    printf("\nExpected: 2 5 7\n");
    SortedLDestroy(list);
}

int main()
{
    TestCreateDestroy();
    TestInsert();
    TestRemove();
    TestSizeIsEmpty();
    TestFind();
    TestFindIf();
    TestIsEqual();
    TestNextPrev();
    TestBeginEnd();
    TestPopFrontBack();
    TestMerge();
    TestForEach();

    return 0;
}

