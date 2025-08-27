/***************************
 * Exercise: PQ Heap
 * Date:     27/08/25
 * Developer: Baruch Haimson
 * Reviewer: tamar
 * Status:   Approved
 ***************************/

#ifndef __ILRD_PQ_H__
#define __ILRD_PQ_H__

#include <stddef.h> /* size_t */

typedef struct pq_heap pq_heap_t;

/* 
*   Creates priority queue.
*   Returns : Empty priority queue if success, NULL on fail
*   Params : pq - If pq is NULL this is undefined behavior
*   Complexity: O(1) 
*/
pq_heap_t* PQHeapCreate(int(*comp)(const void* data1, const void* data2)); 

/* 
*   Destroies Heap and free priority queu
*   Params : pq - If pq is NULL this is undefined behavior
*   Complexity: O(1)
*/
void PQHeapDestroy(pq_heap_t* pq);

/* 
*   Insert new item with data after finding the correct priority queue location
*   Returns : 1 if failed, 0 if succeed
*   Params : pq - If pq is NULL this is undefined behavior, data to enqueued (to be stored)
*   Complexity: O(n)
*/
int PQHeapEnqueue(pq_heap_t* pq, void* data); 

/* 
*   Dequeue from queue the item with highest priority
*   Returns : data of dequed item.
*   Params : pq - If pq is NULL this is undefined behavior.
*   Complexity: O(1)
*/
void* PQHeapDequeue(pq_heap_t* pq);

/* 
*   Returns data of highest priority item.
*   Returns : data of highest priority item.
*   Params : pq - If pq is NULL this is undefined behavior.
*   Complexity: O(1)
*/
void* PQHeapPeek(const pq_heap_t* pq);

/* 
*   Returns data of highest priority item.
*   Returns : 1 if pq is empty, 0 otherwise.
*   Params : pq - If pq is NULL this is undefined behavior.
*   Complexity: O(1)
*/
int PQHeapIsEmpty(const pq_heap_t* pq);

/* 
*   Returns size of items in PQ (Heap)
*   Returns : size of items in priority queue.
*   Params : pq - If pq is NULL this is undefined behavior.
*   Complexity: O(n)
*/
size_t PQHeapSize(const pq_heap_t* pq);

/* 
*   Clears all items in priority queue (Dequeues all items until emptiness)
*   Params : pq - If pq is NULL this is undefined behavior.
*   Complexity: O(n) 
*/
void PQHeapClear(pq_heap_t* pq);

/* 
*   Dequeues particular item with the data we sent
*   Returns: data of erased item if find, erase and success (matches param), NULL otherwise (if not find matched param and didn't erase).
*   Params : pq - If pq is NULL this is undefined behavior, matching function and param (data to match).
*   Complexity: O(n) 
*/
void* PQHeapErase(pq_heap_t* pq, int (*is_match_func)(const void* data, const void* param), const void* param); 

#endif /* __ILRD_PQ_H__ */



