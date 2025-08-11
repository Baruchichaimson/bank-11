#include <stdio.h>

#include "bst.h"

static int action_func(void* data, void* param)
{
    return (*(int*)data == *(int*)param) ? 0 : 1;
}

static int cmp_func(const void* data1, const void* data2)
{
    return (*(int*)data1 - *(int*)data2);
}

void TestCreatDestroyBST(void)
{
    int values[] = {50, 30, 70, 20, 40, 23, 21, 24,60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    size_t n = sizeof(values) / sizeof(values[0]);
    bst_t* tree = BSTCreate(cmp_func);
    if (!tree)
    {
        printf("BSTCreate failed\n");
        return;
    }
    printf("BSTCreate passed\n");

    for (size_t i = 0; i < n; ++i)
    {
        BSTInsert(tree, &values[i]);
    }

    printf("BSTInsert done\n");

    BSTDestroy(tree);
    printf("BSTDestroy passed\n");
    
}

void TestInsertRemoveBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    int data1 = 10, data2 = 20;
    bst_iter_t iter1, iter2;

    iter1 = BSTInsert(tree, &data1);
    if (iter1.node)
    {
        printf("%d\n", *(int*)BSTGetData(iter1));
        printf("BSTInsert passed for data1\n");
    }
    else
    {
        printf("BSTInsert failed for data1\n");
    }

    iter2 = BSTInsert(tree, &data2);
    if (iter2.node)
    {
        printf("BSTInsert passed for data2\n");
    }
    else
    {
        printf("BSTInsert failed for data2\n");
    }

    BSTRemove(iter1);
    printf("BSTRemove passed for data1\n");

    BSTDestroy(tree);
}

void TestSizeIsEmptyBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    if (BSTIsEmpty(tree))
    {
        printf("BSTIsEmpty passed\n");
    }
    else
    {
        printf("BSTIsEmpty failed\n");
    }

    BSTDestroy(tree);
}

void TestBeginEndBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    bst_iter_t begin = BSTBegin(tree);
    bst_iter_t end = BSTEnd(tree);

    if (begin.node == NULL && end.node == NULL)
    {
        printf("BSTBegin and BSTEnd passed\n");
    }
    else
    {
        printf("BSTBegin or BSTEnd failed\n");
    }

    BSTDestroy(tree);
}

void TestNextPrevBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    int data1 = 10, data2 = 20;
    bst_iter_t iter1, iter2;

    iter1 = BSTInsert(tree, &data1);
    iter2 = BSTInsert(tree, &data2);

    bst_iter_t next_iter = BSTNext(iter1);
    if (BSTIterIsSame(next_iter, iter2))
    {
        printf("BSTNext passed\n");
    }
    else
    {
        printf("BSTNext failed\n");
    }

    bst_iter_t prev_iter = BSTPrev(iter2);
    if (BSTIterIsSame(prev_iter, iter1))
    {
        printf("BSTPrev passed\n");
    }
    else
    {
        printf("%d\n", *(int*)BSTGetData(prev_iter));
        printf("%d\n", *(int*)BSTGetData(iter1));
        printf("BSTPrev failed\n");
    }

    BSTDestroy(tree);
}

void TestIterIsSameBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    int data1 = 10, data2 = 20;
    bst_iter_t iter1, iter2;

    iter1 = BSTInsert(tree, &data1);
    iter2 = BSTInsert(tree, &data2);

    if (BSTIterIsSame(iter1, iter1) && !BSTIterIsSame(iter1, iter2))
    {
        printf("BSTIterIsSame passed\n");
    }
    else
    {
        printf("BSTIterIsSame failed\n");
    }

    BSTDestroy(tree);
}

void TestGetDataBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    int data1 = 10, data2 = 20;
    bst_iter_t iter1, iter2;

    iter1 = BSTInsert(tree, &data1);
    iter2 = BSTInsert(tree, &data2);

    if (*(int*)BSTGetData(iter1) == data1 && *(int*)BSTGetData(iter2) == data2)
    {
        printf("BSTGetData passed\n");
    }
    else
    {
        printf("BSTGetData failed\n");
    }

    BSTDestroy(tree);
}

void TestFindBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    int data1 = 10, data2 = 20;
    bst_iter_t iter1, iter2;

    iter1 = BSTInsert(tree, &data1);
    iter2 = BSTInsert(tree, &data2);

    bst_iter_t found_iter = BSTFind(tree, &data1);
    if (BSTIterIsSame(iter1, found_iter))
    {
        printf("BSTFind passed for data1\n");
    }
    else
    {
        printf("BSTFind failed for data1\n");
    }

    found_iter = BSTFind(tree, &data2);
    if (BSTIterIsSame(iter2, found_iter))
    {
        printf("BSTFind passed for data2\n");
    }
    else
    {
        printf("BSTFind failed for data2\n");
    }

    BSTDestroy(tree);
}

void TestForEachBST(void)
{
    bst_t* tree = BSTCreate(cmp_func);
    int data1 = 10, data2 = 20, param = 10;
    bst_iter_t iter1, iter2;

    iter1 = BSTInsert(tree, &data1);
    iter2 = BSTInsert(tree, &data2);

    if (BSTForEach(iter1, iter2, action_func, &param) == 0)
    {
        printf("BSTForEach passed\n");
    }
    else
    {
        printf("BSTForEach failed\n");
    }

    BSTDestroy(tree);
}

int main()
{
    TestCreatDestroyBST();
    TestInsertRemoveBST();
    TestSizeIsEmptyBST();
    TestBeginEndBST();
    TestNextPrevBST();
    TestIterIsSameBST();
    TestGetDataBST();
    TestFindBST();
    TestForEachBST();
}