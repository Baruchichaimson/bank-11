#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"

/* ================== helper functions ================== */
static void PrintArray(int* arr, size_t n);
static int cmp_int(const void *data1, const void *data2);
/* ====================================================== */

void TestBinarySearchIter(void)
{
    int arr1[] = {1, 3, 5, 7, 9};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    int *empty = NULL;
    int idx;

    printf("=== TestBinarySearchIter ===\n");

    /* Case 1: Element exists */
    idx = BinarySerachIter(arr1, n1, 5);
    printf("Find 5 → Expected 2, Got %d\n", idx);

    /* Case 2: Element does not exist */
    idx = BinarySerachIter(arr1, n1, 8);
    printf("Find 8 → Expected -1, Got %d\n", idx);

    /* Case 3: Empty array */
    idx = BinarySerachIter(empty, 0, 1);
    printf("Empty array → Expected -1, Got %d\n", idx);
}

void TestBinarySearchRec(void)
{
    int arr1[] = {2, 4, 6, 8, 10};
    int arr2[] = {42};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    int idx;

    printf("=== TestBinarySearchRec ===\n");

    /* Case 1: Element exists */
    idx = BinarySearchRec(arr1, n1, 10);
    printf("Find 10 → Expected 4, Got %d\n", idx);

    /* Case 2: Element does not exist */
    idx = BinarySearchRec(arr1, n1, 5);
    printf("Find 5 → Expected -1, Got %d\n", idx);

    /* Case 3: Single element array */
    idx = BinarySearchRec(arr2, 1, 42);
    printf("Single element (found) → Expected 0, Got %d\n", idx);

    idx = BinarySearchRec(arr2, 1, 99);
    printf("Single element (not found) → Expected -1, Got %d\n", idx);
}

void TestMergeSort(void)
{
    int arr1[] = {38, 27, 43, 3, 9, 82, 10};
    int arr2[] = {1, 2, 3, 4, 5};
    int arr3[] = {42};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    size_t n2 = sizeof(arr2) / sizeof(arr2[0]);
    int ret;

    printf("=== TestMergeSort ===\n");

    /* Case 1: General case */
    ret = MergeSort(arr1, n1);
    printf("Sorted arr1 → ");
    PrintArray(arr1, n1);

    /* Case 2: Already sorted */
    ret = MergeSort(arr2, n2);
    printf("Already sorted → ");
    PrintArray(arr2, n2);

    /* Case 3: Single element */
    ret = MergeSort(arr3, 1);
    printf("Single element → ");
    PrintArray(arr3, 1);

    if(ret)
    {
        printf("status failed %d ", ret);
    }
}

void TestQuickSort(void)
{
    int arr1[] = {10, 7, 8, 9, 1, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    int arr3[] = {9, 8, 7, 6, 5};
    int arr4[] = {42};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    size_t n2 = sizeof(arr2) / sizeof(arr2[0]);
    size_t n3 = sizeof(arr3) / sizeof(arr3[0]);

    printf("=== TestQuickSort ===\n");

    /* Case 1: General case */
    QuickSort(arr1, n1, sizeof(int), cmp_int);
    printf("Sorted arr1 → ");
    PrintArray(arr1, n1);

    /* Case 2: Already sorted */
    QuickSort(arr2, n2, sizeof(int), cmp_int);
    printf("Already sorted → ");
    PrintArray(arr2, n2);

    /* Case 3: Reverse sorted */
    QuickSort(arr3, n3, sizeof(int), cmp_int);
    printf("Reverse sorted → ");
    PrintArray(arr3, n3);

    /* Case 4: Single element */
    QuickSort(arr4, 1, sizeof(int), cmp_int);
    printf("Single element → ");
    PrintArray(arr4, 1);
}

void TestQuickSortLargeArray(void)
{
    const size_t ARRAY_SIZE = 1000000;
    size_t i = 0;

    clock_t start, end;
    double duration;

    int *arr1 = malloc(sizeof(int) * ARRAY_SIZE);
    int *arr3 = malloc(sizeof(int) * ARRAY_SIZE);

    if (!arr1 || !arr3)
    {
        printf("Memory allocation failed\n");
        return;
    }

    srand((unsigned)time(NULL));
    for (i= 0; i < ARRAY_SIZE; ++i)
    {
        int val = rand();
        arr1[i] = val;
        arr3[i] = val;
    }

    /* Custom QuickSort */
    start = clock();
    QuickSort(arr1, ARRAY_SIZE, sizeof(int), cmp_int);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Custom QuickSort: %f seconds\n", duration);

    /* glibc qsort */
    start = clock();
    qsort(arr3, ARRAY_SIZE, sizeof(int), cmp_int);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("glibc qsort: %f seconds\n", duration);

    free(arr1);
    free(arr3);
}

void TestMergeSortedLists(void)
{
    node_t a1, a2, a3;
    node_t b1, b2, b3;
    node_t* merged;
    node_t* curr;

    /* בניית הרשימה הראשונה: 1 -> 3 -> 5 */
    a1.data = 1; a1.next = &a2;
    a2.data = 3; a2.next = &a3;
    a3.data = 5; a3.next = NULL;

    /* בניית הרשימה השנייה: 2 -> 4 -> 6 */
    b1.data = 2; b1.next = &b2;
    b2.data = 4; b2.next = &b3;
    b3.data = 6; b3.next = NULL;

    /* קריאה לפונקציה */
    merged = MergeSortedLists(&a1, &b1);

    /* הדפסת הרשימה הממוזגת */
    printf("Merged list: ");
    for (curr = merged; curr != NULL; curr = curr->next)
    {
        printf("%d ", curr->data);
    }
    printf("\n");
}


int main(void)
{
    TestBinarySearchIter();
    TestBinarySearchRec();
    TestMergeSort();
    TestQuickSort();
    TestQuickSortLargeArray();
    TestMergeSortedLists();

    return 0;
}

/* ================== helpers functions ================== */
static void PrintArray(int* arr, size_t n)
{
    size_t i;
    for (i = 0; i < n; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

static int cmp_int(const void *data1, const void *data2)
{
    int a = *(const int *)data1;
    int b = *(const int *)data2;

    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}
