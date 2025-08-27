/***************************
 * Exercise: PQ Heap
 * Date:     27/08/25
 * Developer: Baruch Haimson
 * Reviewer: tamar
 * Status:   Approved
 ***************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "pq_heap.h"   /* pq_heap_t */
#include "heap.h"   /* HeapCreate */

struct pq_heap
{
    heap_t* heap;
};

pq_heap_t* PQHeapCreate(int(*comp)(const void* data1, const void* data2))
{
    pq_heap_t* pq = NULL;

    assert(comp);

    pq = (pq_heap_t*)malloc(sizeof(pq_heap_t));
    if (NULL == pq)
    {
        return NULL;
    }

    pq->heap = HeapCreate(comp);
    if (NULL == pq->heap)
    {
        free(pq);
        return NULL;
    }

    return pq;
}

void PQHeapDestroy(pq_heap_t* pq)
{
    assert(pq);

    HeapDestroy(pq->heap);
    pq->heap = NULL;

    free(pq);
    pq = NULL;
}


int PQHeapEnqueue(pq_heap_t* pq, void* data)
{
    assert(pq);

    return HeapPush(pq->heap, data); 
}

void* PQHeapDequeue(pq_heap_t* pq)
{
    void* data = NULL;

    assert(pq);
    assert(!PQHeapIsEmpty(pq));

    data = PQHeapPeek(pq);    
    HeapPop(pq->heap);    

    return data;
}

void* PQHeapPeek(const pq_heap_t* pq)
{
    assert(pq);
    assert(!PQHeapIsEmpty(pq));

    return HeapPeek(pq->heap);
}

int PQHeapIsEmpty(const pq_heap_t* pq)
{
    assert(pq);

    return HeapIsEmpty(pq->heap);
}

size_t PQHeapSize(const pq_heap_t* pq)
{
    assert(pq);

    return HeapSize(pq->heap);
}

void PQHeapClear(pq_heap_t* pq)
{
    assert(pq);

    while (!PQHeapIsEmpty(pq))
    {
        PQHeapDequeue(pq);
    }
}

void* PQHeapErase(pq_heap_t* pq, int (*is_match_func)(const void* data, const void* param), const void* param)
{
    assert(pq);
    assert(is_match_func);

    return HeapRemove(pq->heap, param, is_match_func);
}

