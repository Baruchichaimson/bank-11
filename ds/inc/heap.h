/***************************
 Exercise:  heap
 Date: 	    27/08/25
 Developer: Baruch Haimson
 Reviewer:  Tamar
 Status:    Approved
***************************/

#ifndef __ILRD_HEAP__   
#define __ILRD_HEAP__

#include <stddef.h> /* size_t */

typedef int (*is_match_func)(const void* data1, const void* data2);

typedef int (*compare_func)(const void* data1, const void* data2);

typedef struct heap heap_t;

/* ---------------------------------------------------------------
   Input:  A compare_func to define priority ordering.
   Output: Pointer to a new empty heap, or NULL on allocation failure.
   Action: Allocates and initializes a heap structure.
   Why defined: Entry point for creating a heap object.
   Time Complexity: O(1).
*/
heap_t* HeapCreate(compare_func func);

/* ---------------------------------------------------------------
   Input:  Pointer to a heap.
   Output: None.
   Action: Frees all memory used by the heap.
   Why defined: Ensures proper cleanup of heap resources.
   Time Complexity: O(1).
*/
void HeapDestroy(heap_t* heap);

/* ---------------------------------------------------------------
   Input:  Const pointer to a heap.
   Output: Pointer to the data at the root (highest priority).
   Action: Returns the top element without removing it.
   Why defined: Allows access to the current top of the heap.
   Time Complexity: O(1).
*/
void* HeapPeek(const heap_t* heap);

/* ---------------------------------------------------------------
   Input:  Pointer to a heap, and a data element to insert.
   Output: 0 on success, 1 on failure.
   Action: Inserts the element into the heap and maintains ordering.
   Why defined: Adds elements dynamically to the heap.
   Time Complexity: O(log n).
*/
int HeapPush(heap_t* heap, void* data);

/* ---------------------------------------------------------------
   Input:  Pointer to a heap.
   Output: None.
   Action: Removes the root element (highest priority) from the heap 
           and rebalances the structure.
   Why defined: Standard heap operation for priority queue usage.
   Time Complexity: O(log n).
*/
void HeapPop(heap_t* heap);

/* ---------------------------------------------------------------
   Input:  Pointer to a heap, a target data element, and an is_match_func.
   Output: Pointer to the removed data if found, NULL if not found.
   Action: Searches for an element matching the given criteria, removes it, 
           and rebalances the heap.
   Why defined: Provides ability to remove arbitrary elements, not just root.
   Time Complexity: O(n) in the worst case (search) + O(log n) (rebalance).
*/
void* HeapRemove(heap_t* heap, const void* data, is_match_func func);

/* ---------------------------------------------------------------
   Input:  Const pointer to a heap.
   Output: 1 if heap is empty, 0 otherwise.
   Action: Checks whether the heap contains any elements.
   Why defined: Utility for control flow and safety checks.
   Time Complexity: O(1).
*/
int HeapIsEmpty(const heap_t* heap);

/* ---------------------------------------------------------------
   Input:  Const pointer to a heap.
   Output: Number of elements currently stored in the heap.
   Action: Returns the heap’s size.
   Why defined: Useful for iteration, debugging, and validation.
   Time Complexity: O(1).
*/
size_t HeapSize(const heap_t* heap);

#endif /* __ILRD_HEAP__ */
