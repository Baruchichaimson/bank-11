#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sort.h"

/*================ helper functions ==================================*/

static int BinarySearchRecHelp(int* arr, int start, int end, int target);
static int Merge(int *arr, int left, int mid, int right);
static int MergeSortHelper(int *arr, int left, int right);
static size_t Partition(void *arr, size_t low, size_t high, size_t size_of_element, int (*cmp)(const void *data1, const void *data2));
static void QuickSortHelper(void *arr, size_t low, size_t high, size_t size_of_element, int (*cmp)(const void *data1, const void *data2));

/*=====================================================================*/

int BinarySerachIter(int* arr, size_t n, int target)
{
    size_t start = 0;
    size_t end = n - 1;
    size_t mid = 0;

    while(start <= end && n > 0)
    {
        mid = start + (end - start) / 2;

        if(arr[mid] == target)
        {
            return mid;
        }
        else if(arr[mid] < target)
        {
            start = mid + 1;
        }
        else
        {
            if(mid == 0)
            {
                break;
            }
            end = mid - 1;
        }
    }
    return -1;
}

int BinarySearchRec(int* arr, size_t n, int target)
{
    if(!arr || n == 0)
    {
        return -1;
    }
    return BinarySearchRecHelp(arr, 0, n - 1, target);
}

int MergeSort(int* arr_to_sort, size_t num_elements)
{
    assert(arr_to_sort);
    assert(num_elements);

    return MergeSortHelper(arr_to_sort, 0, num_elements - 1);
}

void QuickSort(void *arr, size_t n, size_t size_of_element, int (*cmp)(const void *data1, const void *data2))
{
    if (!arr || n == 0 || !cmp)
        return;

    QuickSortHelper(arr, 0, n - 1, size_of_element, cmp);
}

node_t* MergeSortedLists(node_t* head1, node_t* head2)
{
    node_t dummy;
    node_t *tail = &dummy;
    dummy.next = NULL;

    while (head1 && head2)
    {
        if (head1->data <= head2->data)
        {
            tail->next = head1;
            head1 = head1->next;
        }
        else
        {
            tail->next = head2;
            head2 = head2->next;
        }
        tail = tail->next;
    }

    tail->next = (head1) ? head1 : head2;

    return dummy.next;
}

/*================ helper functions ==================================*/

static int BinarySearchRecHelp(int* arr, int start, int end, int target)
{
    int mid = 0;
    if(start > end)
    {
        return -1;
    }

    mid = start + (end - start) / 2;

    if(arr[mid] == target)
    {
        return mid;
    }
    else if(arr[mid] < target)
    {
        return BinarySearchRecHelp(arr, mid + 1, end, target);
    }
    else
    {
        return BinarySearchRecHelp(arr, start, mid - 1, target);
    }
}

static int Merge(int *arr, int left, int mid, int right)
{
    size_t Lsize = mid - left + 1;
    size_t Rsize = right - mid;
    size_t k = left;
    size_t i = 0;
    size_t j = 0;
    int *Larr = NULL;
    int *Rarr = NULL;

    Larr = malloc(Lsize * sizeof(int));
    if(!Larr)
    {
        return -1;
    }
    Rarr = malloc(Rsize * sizeof(int));
    if(!Rarr)
    {
        return -1;
    }

    memcpy(Larr, &arr[left], Lsize * sizeof(int));
    memcpy(Rarr, &arr[mid + 1], Rsize * sizeof(int));

    while (i < Lsize && j < Rsize)
    {
        if (Larr[i] <= Rarr[j])
        {
            arr[k++] = Larr[i++];
        }
        else
        {
            arr[k++] = Rarr[j++];
        }
    }

    while (i < Lsize)
    {
         arr[k++] = Larr[i++];
    }
    while (j < Rsize) 
    {
        arr[k++] = Rarr[j++];
    }

    free(Larr);
    free(Rarr);

    return 0;
}

static int MergeSortHelper(int *arr, int left, int right)
{
    int mid = 0;
    int status = 0;

    if (left < right)
    {
        mid = left + (right - left) / 2;

        status = MergeSortHelper(arr, left, mid);
        if(status != 0)
        {
            return status; 
        }

        status = MergeSortHelper(arr, mid + 1, right);
        if(status != 0)
        {
            return status; 
        }

        return Merge(arr, left, mid, right);
    }
    return status;
}

static void Swap(void *a, void *b, size_t size)
{
    size_t i;
    char tmp;
    char *pa = (char*)a;
    char *pb = (char*)b;

    for (i = 0; i < size; ++i)
    {
        tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

static size_t Partition(void *arr, size_t low, size_t high, size_t size_of_element, int (*cmp)(const void *data1, const void *data2))
{
    char *array = (char *)arr;
    char *pivot = array + high * size_of_element;
    size_t i = low - 1;
    size_t j;

    for (j = low; j < high; ++j)
    {
        if (cmp(array + j * size_of_element, pivot) <= 0)
        {
            ++i;
            Swap(array + i * size_of_element, array + j * size_of_element, size_of_element);
        }
    }

    ++i;
    Swap(array + i * size_of_element, pivot, size_of_element);

    return i;
}


static void QuickSortHelper(void *arr, size_t low, size_t high, size_t size_of_element, int (*cmp)(const void *data1, const void *data2))
{
    size_t pi;
    if (low < high)
    {
        pi = Partition(arr, low, high, size_of_element, cmp);
        if (pi > 0) 
        {
            QuickSortHelper(arr, low, pi - 1, size_of_element, cmp);
        }
        QuickSortHelper(arr, pi + 1, high, size_of_element, cmp);
    }
}