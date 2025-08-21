/************************************
 * Exercise: AVL 
 * Date: 18/08/25
 * Developer: Baruch Haimson
 * Reviewer: Menny
 * Status: Aprroved
 ************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avl.h"

/* ===== Helper Function ===== */
int IntCmp(const void* a, const void* b);
int PrintNode(void* data, void* param);
/*=============================*/

void TestAVLCreate()
{
    avl_t* tree;

    printf("---- Test AVL Create ----\n");
    tree = AVLCreate(IntCmp);
    printf("Tree created -> %s\n", (tree != NULL) ? "SUCCESS" : "FAIL");
    AVLDestroy(tree);
}

void TestAVLDestroy()
{
    avl_t* tree;

    printf("---- Test AVL Destroy ----\n");
    tree = AVLCreate(IntCmp);
    AVLDestroy(tree);
    printf("Tree destroyed (no segfault) -> SUCCESS\n");
}

void TestAVLInsert()
{
    avl_t* tree;
    int arr[] = {10, 20, 30, 40, 50, 25};
    size_t i;

    printf("---- Test AVL Insert ----\n");

    tree = AVLCreate(IntCmp);

    for (i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
    {
        AVLInsert(tree, &arr[i]);
    }

    printf("Count: %lu (expected 6)\n", AVLCount(tree));
    printf("Height: %lu (expected 3)\n", AVLHeight(tree));
    printf("Inorder (expected: 10 20 25 30 40 50): ");
    AVLForEach(tree, AVL_IN_ORDER, PrintNode, NULL);
    printf("\n\n");

    AVLDestroy(tree);
}

void TestAVLRemove()
{
    avl_t* tree;
    int arr[] = {10, 20, 30, 40, 50, 25};
    size_t i;

    printf("---- Test AVL Remove ----\n");

    tree = AVLCreate(IntCmp);

    for (i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
    {
        AVLInsert(tree, &arr[i]);
    }

    AVLRemove(tree, &arr[5]); 
    printf("After removing 25: Count=%lu (expected 5) | Inorder: ",
           AVLCount(tree));
    AVLForEach(tree, AVL_IN_ORDER, PrintNode, NULL);
    printf("\n");

    AVLRemove(tree, &arr[4]); 
    printf("After removing 50: Count=%lu (expected 4) | Inorder: ",
           AVLCount(tree));
    AVLForEach(tree, AVL_IN_ORDER, PrintNode, NULL);
    printf("\n");

    AVLRemove(tree, &arr[1]); 
    printf("After removing 20: Count=%lu (expected 3) | Inorder: ",
           AVLCount(tree));
    AVLForEach(tree, AVL_IN_ORDER, PrintNode, NULL);
    printf("\n\n");

    AVLDestroy(tree);
}

void TestAVLHeight()
{
    avl_t* tree;
    int vals[] = {1, 2, 3, 4};
    int i;

    printf("---- Test AVL Height ----\n");

    tree = AVLCreate(IntCmp);

    printf("Empty tree height = %lu (expected 0)\n", AVLHeight(tree));

    for (i = 0; i < 4; ++i)
    {
        AVLInsert(tree, &vals[i]);
    }

    printf("Tree height after inserts = %lu (expected 3)\n", AVLHeight(tree));

    AVLDestroy(tree);
}

void TestAVLCount()
{
    avl_t* tree;
    int vals[] = {5, 3, 7};
    int i;

    printf("---- Test AVL Count ----\n");

    tree = AVLCreate(IntCmp);

    printf("Empty tree count = %lu (expected 0)\n", AVLCount(tree));

    for (i = 0; i < 3; ++i)
    {
        AVLInsert(tree, &vals[i]);
    }

    printf("Count after 3 inserts = %lu (expected 3)\n", AVLCount(tree));

    AVLDestroy(tree);
}

void TestAVLIsEmpty()
{
    avl_t* tree;
    int val = 10;

    printf("---- Test AVL IsEmpty ----\n");

    tree = AVLCreate(IntCmp);
    printf("Tree empty? %s (expected YES)\n", AVLIsEmpty(tree) ? "YES" : "NO");

    AVLInsert(tree, &val);
    printf("Tree empty after insert? %s (expected NO)\n",
           AVLIsEmpty(tree) ? "YES" : "NO");

    AVLDestroy(tree);
}

void TestAVLFind()
{
    avl_t* tree;
    int arr[] = {15, 10, 20, 8, 12};
    int key1 = 12, key2 = 30;
    int* found1;
    int* found2;
    int i;

    printf("---- Test AVL Find ----\n");

    tree = AVLCreate(IntCmp);

    for (i = 0; i < 5; ++i)
    {
        AVLInsert(tree, &arr[i]);
    }

    found1 = (int*)AVLFind(tree, &key1);
    found2 = (int*)AVLFind(tree, &key2);

    printf("Find 12 -> %s (expected FOUND)\n", (found1 ? "FOUND" : "NOT FOUND"));
    printf("Find 30 -> %s (expected NOT FOUND)\n", (found2 ? "FOUND" : "NOT FOUND"));

    AVLDestroy(tree);
}

void TestAVLForEach()
{
    avl_t* tree;
    int arr[] = {4, 2, 6, 1, 3, 5, 7};
    int i;

    printf("---- Test AVL ForEach ----\n");

    tree = AVLCreate(IntCmp);

    for (i = 0; i < 7; ++i)
    {
        AVLInsert(tree, &arr[i]);
    }

    printf("Inorder traversal (expected: 1 2 3 4 5 6 7): ");
    AVLForEach(tree, AVL_IN_ORDER, PrintNode, NULL);
    printf("\n");

    printf("Preorder traversal: ");
    AVLForEach(tree, AVL_PRE_ORDER, PrintNode, NULL);
    printf("\n");

    printf("Postorder traversal: ");
    AVLForEach(tree, AVL_POST_ORDER, PrintNode, NULL);
    printf("\n\n");

    AVLDestroy(tree);
}

int main()
{
    TestAVLCreate();
    TestAVLDestroy();
    TestAVLInsert();
    TestAVLRemove();
    TestAVLHeight();
    TestAVLCount();
    TestAVLIsEmpty();
    TestAVLFind();
    TestAVLForEach();

    return 0;
}

/* ===== Helper Function ===== */
int IntCmp(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}
int PrintNode(void* data, void* param)
{
    (void)param;
    printf("%d ", *(int*)data);
    return 0;
}

