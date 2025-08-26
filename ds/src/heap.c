#include <stdlib.h>
#include <assert.h>

#include "heap.h"
#include "vector.h"

#define DEFULT_CAPACITY (128)
#define SIZE_ELEMENT  (sizeof(void*))

struct heap 
{
    compare_func compare;
    vector_t* vector;
};

/*===================== helper functions ===================*/

static size_t Parent(size_t i);
static size_t Left(size_t i);
static size_t Right(size_t i);
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

    VectorDestroy(heap->vector);

    free(heap);
}

void* HeapPeek(const heap_t* heap)
{
    assert(heap);
    assert(!HeapIsEmpty(heap));

    return *(void **)VectorGetAccessToElement(heap->vector, 0);
}

int HeapPush(heap_t* heap, void* data)
{
    assert(heap);
    
    if (VectorPushBack(heap->vector, &data) != 0)
    {
        return 1; 
    }

    HeapifyUp(heap, VectorSize(heap->vector) - 1);
    return 0;
}

void HeapPop(heap_t* heap)
{
    void **root  = NULL;
    void **last = NULL;
    size_t size = VectorSize(heap->vector);

    assert(heap);

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

void HeapRemove(heap_t* heap, const void* data, is_match_func func)
{
    size_t i = 0;
    void **curr = NULL;
    void **last = NULL;
    size_t size = VectorSize(heap->vector);

    assert(heap);

    for (i = 0; i < size; ++i)
    {
        curr = VectorGetAccessToElement(heap->vector, i);
        if (func(*curr, data))
        {
            last = VectorGetAccessToElement(heap->vector, size - 1);
            *curr = *last;
            VectorPopBack(heap->vector);

            if (i < VectorSize(heap->vector))
            {
                HeapifyDown(heap, i);
                HeapifyUp(heap, i);
            }
            return;
        }
    }
}

int HeapIsEmpty(const heap_t* heap)
{
    assert(heap);

    return VectorSize(heap->vector) == 0;
}

size_t HeapSize(const heap_t* heap)
{
    assert(heap);

    return VectorSize(heap->vector);
}

/*===================== helper functions ===================*/

static size_t Parent(size_t i) 
{ 
    return (i - 1) / 2; 
}

static size_t Left(size_t i)   
{ 
    return (2 * i) + 1; 
}

static size_t Right(size_t i)  
{ 
    return (2 * i) + 2; 
}

static void Swap(void **a, void **b)
{
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

static void HeapifyUp(heap_t *heap, size_t index)
{
    void **curr = NULL;
    void **parent = NULL;
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
    size_t size = VectorSize(heap->vector);
    size_t left = 0;
    size_t right = 0;
    size_t largest = 0;
    void **curr = NULL;
    void **l = NULL;
    void **r = NULL;
    void **a = NULL;
    void **b = NULL;

    while (1)
    {
        left = Left(index);
        right = Right(index);
        largest = index;

        curr = VectorGetAccessToElement(heap->vector, index);

        if (left < size)
        {
            l = VectorGetAccessToElement(heap->vector, left);
            if (heap->compare(*l, *curr) > 0)
            {
                largest = left;
                curr = l;
            }
        }

        if (right < size)
        {
            r = VectorGetAccessToElement(heap->vector, right);
            l = VectorGetAccessToElement(heap->vector, largest);
            if (heap->compare(*r, *l) > 0)
            {
                largest = right;
            }
        }

        if (largest != index)
        {
            a = VectorGetAccessToElement(heap->vector, index);
            b = VectorGetAccessToElement(heap->vector, largest);
            Swap(a, b);
            index = largest;
        }
        else
        {
            break;
        }
    }
}
