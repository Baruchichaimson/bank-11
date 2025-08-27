/***************************
 Exercise:  heap
 Date: 	    27/08/25
 Developer: Baruch Haimson
 Reviewer:  Tamar
 Status:    Approved
***************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "heap.h" /* API */
#include "vector.h" /* VectorCreate*/

#define DEFULT_CAPACITY (128)
#define SIZE_ELEMENT  (sizeof(void*))

struct heap 
{
    compare_func compare;
    vector_t* vector;
};

/*===================== helper functions ===================*/

static size_t Parent(size_t i);
static size_t LeftChild(size_t i);
static size_t RightChild(size_t i);
static void Swap(void **a, void **b);
static void HeapifyUp(heap_t *heap, size_t index);
static void HeapifyDown(heap_t *heap, size_t index);

/*==========================================================*/

heap_t* HeapCreate(compare_func func)
{
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) 
    {
        return NULL;
    }

    heap->vector = VectorCreate(DEFULT_CAPACITY, SIZE_ELEMENT);
    if (!heap->vector)
    {
        free(heap);
        return NULL;
    }

    heap->compare = func;
    return heap;
}

void HeapDestroy(heap_t* heap)
{
    assert(heap);
    assert(heap->vector);

    VectorDestroy(heap->vector);

    free(heap);
}

void* HeapPeek(const heap_t* heap)
{
    assert(heap);
    assert(!HeapIsEmpty(heap));
    assert(heap->vector);

    return *(void **)VectorGetAccessToElement(heap->vector, 0);
}

int HeapPush(heap_t* heap, void* data)
{
    assert(heap);
    assert(data);
    assert(heap->vector);
    
    if (VectorPushBack(heap->vector, &data) != 0)
    {
        return 1; 
    }

    HeapifyUp(heap, HeapSize(heap) - 1);
    return 0;
}

void HeapPop(heap_t* heap)
{
    void** root  = NULL;
    void** last = NULL;
    size_t size = HeapSize(heap);

    assert(heap);
    assert(!HeapIsEmpty(heap)); 
    assert(heap->vector);

    if (size == 0) 
    {
        return;
    }

    root = VectorGetAccessToElement(heap->vector, 0);
    last = VectorGetAccessToElement(heap->vector, size - 1);

    *root = *last;
    VectorPopBack(heap->vector);

    if (!HeapIsEmpty(heap))
    {
        HeapifyDown(heap, 0);
    }
}

void* HeapRemove(heap_t* heap, const void* data, is_match_func func)
{
    size_t i = 0;
    void **curr = NULL;
    void **last = NULL;
    size_t size = 0;
    void *removed = NULL;

    assert(heap);

    size = HeapSize(heap);

    for (i = 0; i < size; ++i)
    {
        curr = VectorGetAccessToElement(heap->vector, i);
        if (func(*curr, data))
        {
            removed = *curr;

            last = VectorGetAccessToElement(heap->vector, size - 1);

            *curr = *last;
            VectorPopBack(heap->vector);

            if (i > 0 && heap->compare(curr, VectorGetAccessToElement(heap->vector, Parent(i))) > 0)
            {
                HeapifyUp(heap, i);
            }
            else
            {
                HeapifyDown(heap, i);
            }
            return removed;
        }
    }
    return NULL;
}

int HeapIsEmpty(const heap_t* heap)
{
    assert(heap);
    assert(heap->vector);

    return HeapSize(heap) == 0;
}

size_t HeapSize(const heap_t* heap)
{
    assert(heap);
    assert(heap->vector);

    return VectorSize(heap->vector);
}

/*===================== helper functions ===================*/

static size_t Parent(size_t i) 
{ 
    return (i - 1) / 2; 
}

static size_t LeftChild(size_t i)   
{ 
    return (2 * i) + 1; 
}

static size_t RightChild(size_t i)  
{ 
    return (2 * i) + 2; 
}

static void Swap(void** a, void** b)
{
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}

static void HeapifyUp(heap_t *heap, size_t index)
{
    void** curr = NULL;
    void** parent = NULL;
    while (index > 0)
    {
        curr = VectorGetAccessToElement(heap->vector, index);
        parent = VectorGetAccessToElement(heap->vector, Parent(index));

        if (heap->compare(*curr, *parent) > 0) 
        {
            Swap(curr, parent);
            index = Parent(index);
        }
        else
        {
            break;
        }
    }
}

static void HeapifyDown(heap_t *heap, size_t index)
{
    size_t size = HeapSize(heap);
    size_t Left = 0;
    size_t Right = 0;
    size_t largest = 0;
    void** curr = NULL;
    void** l = NULL;
    void** r = NULL;
    void** index_data = NULL;
    void** largest_data = NULL;

    while (1)
    {
        Left = LeftChild(index);
        Right = RightChild(index);
        largest = index;

        curr = VectorGetAccessToElement(heap->vector, index);

        if (Left < size)
        {
            l = VectorGetAccessToElement(heap->vector, Left);
            if (heap->compare(*l, *curr) > 0)
            {
                largest = Left;
                curr = l;
            }
        }

        if (Right < size)
        {
            r = VectorGetAccessToElement(heap->vector, Right);
            l = VectorGetAccessToElement(heap->vector, largest);
            if (heap->compare(*r, *l) > 0)
            {
                largest = Right;
            }
        }

        if (largest != index)
        {
            index_data = VectorGetAccessToElement(heap->vector, index);
            largest_data = VectorGetAccessToElement(heap->vector, largest);
            Swap(index_data, largest_data);
            index = largest;
        }
        else
        {
            break;
        }
    }
}
