/***************************
 Exercise:  dll
 Date: 	    09/07/25
 Developer: Baruch Haimson
 Reviewer:  Lotem
 Status:    Approved
***************************/

#include <stdio.h>
#include <stdlib.h>
#include "dll.h"

typedef enum { SUCCESS = 0, FALIURE = -1, PARTIAL_FALUIRE = -2 } status_t;

void TestPushPop();
void TestInsertRemove();
void TestFindMultiFind();
void TestForEach();
void TestIsEmptyAndCount();
void TestSplice();
void PrintList(const dll_t* list);
void PrintResult(int condition, const char* msg);

int MatchInt(const void* data, const void* param)
{
    return (*(int*)data == *(int*)param);
}

int Count(void* data, void* param)
{
    (void)data;
    ++*(size_t*)param;
    return 1;
}

int PrintInt(void* data, void* param)
{
    (void)param;
    printf("%d ", *(int*)data);
    return 1;
}

void TestPushPop()
{
    printf("\n-- TestPushPop --\n");
    int x = 10, y = 20, z = 30;
    dll_t* list = DLLCreate();
    DLLPushBack(list, &x);
    DLLPushBack(list, &y);
    DLLPushBack(list, &z);

    PrintResult(*(int*)DLLGetData(DLLBegin(list)) == 10, "PushFront 10");
    PrintResult(*(int*)DLLGetData(DLLNext(DLLBegin(list))) == 20, "PushBack 20");

    DLLPopBack(list);
    PrintResult(*(int*)DLLGetData(DLLNext(DLLBegin(list))) == 20, "PopBack -> 20");

    DLLPopBack(list);
    PrintResult(*(int*)DLLGetData(DLLBegin(list)) == 10, "PopFront -> 10");

    DLLDestroy(list);
}

void TestInsertRemove()
{
    printf("\n-- TestInsertRemove --\n");
    int a = 1, b = 2;
    dll_t* list = DLLCreate();

    dll_iter_t iter = DLLPushBack(list, &a);
    dll_iter_t inserted = DLLInsert(list, iter, &b);

    PrintResult(*(int*)DLLGetData(inserted) == 2, "Insert before 1");

    DLLRemove(inserted);
    PrintResult(*(int*)DLLGetData(DLLBegin(list)) == 1, "Removed 2, first is 1");

    DLLDestroy(list);
}

void TestFindMultiFind()
{
    printf("\n-- TestFindMultiFind --\n");
    int arr[] = {1, 2, 3, 2, 4};
    int key = 2;
    dll_t* list = DLLCreate();
    dll_t* output = DLLCreate();

    for (size_t i = 0; i < 5; ++i)
    {
        DLLPushBack(list, &arr[i]);
    }

    dll_iter_t found = DLLFind(DLLBegin(list), DLLEnd(list), MatchInt, &key);
    PrintResult(*(int*)DLLGetData(found) == 2, "Find first 2");

    int result = DLLMultiFind(DLLBegin(list), DLLEnd(list), MatchInt, &key, output);

    size_t count = DLLCount(output);

    PrintResult(count == 2, "MultiFind count == 2");

    printf("MultiFind return code: %d (0=SUCCESS, -1 = FALIURE, -2 = PARTIAL)\n", result);

    DLLDestroy(list);
    DLLDestroy(output);
}


void TestForEach()
{
    printf("\n-- TestForEach --\n");
    int arr[] = {5, 6, 7};
    dll_t* list = DLLCreate();
    for (int i = 0; i < 3; ++i)
    {
        DLLPushBack(list, &arr[i]);
    }

    printf("ForEach prints: ");
    DLLForEach(DLLBegin(list), DLLEnd(list), PrintInt, NULL);
    printf("\n");

    DLLDestroy(list);
}

void TestIsEmptyAndCount()
{
    printf("\n-- TestIsEmptyAndCount --\n");
    dll_t* list = DLLCreate();
    PrintResult(DLLIsEmpty(list), "List is empty");

    int x = 5;
    DLLPushBack(list, &x);
    PrintResult(!DLLIsEmpty(list), "List is not empty");

    size_t count = DLLCount(list);
    PrintResult(count == 1, "Count == 1");

    DLLDestroy(list);
}

void TestSplice()
{
    printf("\n-- TestSplice --\n");
    int a = 1, b = 2, c = 3;
    int d = 4, e = 5;

    dll_t* list1 = DLLCreate();
    dll_t* list2 = DLLCreate();

    DLLPushBack(list1, &a);
    DLLPushBack(list1, &b);
    DLLPushBack(list1, &c);

    DLLPushBack(list2, &d);
    DLLPushBack(list2, &e);

    printf("List1 before splice: ");
    PrintList(list1);
    printf("List2 before splice: ");
    PrintList(list2);

    dll_iter_t from = DLLBegin(list1);               
    dll_iter_t to = DLLNext(DLLNext(from));     

	dll_iter_t from1 = DLLBegin(list2);               
   
    dll_iter_t where = DLLNext(from1);             

    DLLSplice(where, from, to);

    printf("List1 after splice: ");
    PrintList(list1);
    printf("List2 after splice: ");
    PrintList(list2);

    PrintResult(DLLCount(list1) == 1, "List1 count after splice == 1");
    PrintResult(DLLCount(list2) == 4, "List2 count after splice == 4");

    DLLDestroy(list1);
    DLLDestroy(list2);
}

void PrintList(const dll_t* list)
{
    dll_iter_t iter = DLLBegin(list);
    dll_iter_t end = DLLEnd(list);

    while (!DLLIsEqual(iter, end))
    {
        printf("%d ", *(int*)DLLGetData(iter));
        iter = DLLNext(iter);
    }
    printf("\n");
}

void PrintResult(int condition, const char* msg)
{
    printf("%s: %s\n", msg, condition ? "PASS" : "FAIL");
}

int main()
{
    printf("==== DLL Test Suite ====");

    TestPushPop();
    TestInsertRemove();
    TestFindMultiFind();
    TestForEach();
    TestIsEmptyAndCount();
    TestSplice();

    return 0;
}

