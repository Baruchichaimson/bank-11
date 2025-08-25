#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h> /* size_t */

typedef struct node
{
    int data;
    struct node* next;
} node_t;


int BinarySerachIter(int* arr, size_t n, int target);
int BinarySearchRec(int* arr, size_t n, int target);
int MergeSort(int* arr_to_sort, size_t num_elements);
void QuickSort(void *arr, size_t n, size_t size_of_element, int (*cmp)(const void *data1, const void *data2));
node_t* MergeSortedLists(node_t* head1, node_t* head2);

#endif /* __SORT_H__ */
