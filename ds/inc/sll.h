/***************************
 Exercise:  sll
 Date: 	    04/07/25
 Developer: Baruch Haimson
 Reviewer:  Yael
 Status:    Approved
***************************/

#ifndef _ILRD_SLL_
#define _ILRD_SLL_

typedef struct Sll sll_t;
typedef struct Node node_t;

typedef node_t* sll_iter_t;

typedef int (*is_match_func_t)(const void* data, void* param);
typedef int (*action_func_t)(void* data, void* param);

#include <stddef.h>

/*---------------------------------------------------------
 * Function: SLLCreate
 * --------------------------------------------------------
 * Receives:
 *     - None.
 * 
 * Does:
 *     - Allocates and initializes an empty singly linked list with a dummy node.
 *     - The dummy node stores a pointer to the list in its `data`.
 * 
 * Returns:
 *     - Pointer to the created list, or NULL on failure.
 * 
 * Undefined Behavior:
 *     - None.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
sll_t* SLLCreate(void);

/*---------------------------------------------------------
 * Function: SLLDestroy
 * --------------------------------------------------------
 * Receives:
 *     - list: pointer to the list to destroy.
 * 
 * Does:
 *     - Frees all nodes in the list and the list struct itself.
 * 
 * Returns:
 *     - None.
 * 
 * Undefined Behavior:
 *     - If list is NULL.
 * 
 * Time Complexity:
 *     - O(n)
 ---------------------------------------------------------*/
void SLLDestroy(sll_t* list);

/*---------------------------------------------------------
 * Function: SLLBegin
 * --------------------------------------------------------
 * Receives:
 *     - list: pointer to list.
 * 
 * Does:
 *     - Returns iterator to the first logical node in the list.
 *     - If the list is empty, returns the dummy node.
 * 
 * Returns:
 *     - Iterator to the head node.
 * 
 * Undefined Behavior:
 *     - If list is NULL.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
sll_iter_t SLLBegin(const sll_t* list);

/*---------------------------------------------------------
 * Function: SLLEnd
 * --------------------------------------------------------
 * Receives:
 *     - list: pointer to list.
 * 
 * Does:
 *     - Returns iterator to the dummy (tail) node.
 * 
 * Returns:
 *     - Iterator to dummy node.
 * 
 * Undefined Behavior:
 *     - If list is NULL.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
sll_iter_t SLLEnd(const sll_t* list);

/*---------------------------------------------------------
 * Function: SLLNext
 * --------------------------------------------------------
 * Receives:
 *     - iter: an iterator.
 * 
 * Does:
 *     - Returns the iterator after the given one.
 * 
 * Returns:
 *     - Next iterator.
 * 
 * Undefined Behavior:
 *     - If iter is NULL or SLLEnd.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
sll_iter_t SLLNext(sll_iter_t iter);

/*---------------------------------------------------------
 * Function: SLLIsEqual
 * --------------------------------------------------------
 * Receives:
 *     - iter1, iter2: two iterators to compare.
 * 
 * Does:
 *     - Compares the addresses of the two iterators.
 * 
 * Returns:
 *     - 1 if equal, 0 otherwise.
 * 
 * Undefined Behavior:
 *     - None.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
int SLLIsEqual(sll_iter_t iter1, sll_iter_t iter2);

/*---------------------------------------------------------
 * Function: SLLGetData
 * --------------------------------------------------------
 * Receives:
 *     - iter: iterator to node.
 * 
 * Does:
 *     - Returns pointer to data stored in node.
 * 
 * Returns:
 *     - Pointer to data.
 * 
 * Undefined Behavior:
 *     - If iter is NULL or SLLEnd.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
void* SLLGetData(sll_iter_t iter);

/*---------------------------------------------------------
 * Function: SLLSetData
 * --------------------------------------------------------
 * Receives:
 *     - iter: iterator to node.
 *     - data: new data pointer to store in node.
 * 
 * Does:
 *     - Sets node’s data to new value.
 * 
 * Returns:
 *     - None.
 * 
 * Undefined Behavior:
 *     - If iter is NULL or SLLEnd or data is NULL.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
void SLLSetData(sll_iter_t iter, void* data);

/*---------------------------------------------------------
 * Function: SLLInsert
 * --------------------------------------------------------
 * Receives:
 *     - where: iterator to insert before.
 *     - data: data pointer to insert.
 * 
 * Does:
 *     - Allocates new node and inserts it before 'where' using data swap trick.
 *     - If inserting before the dummy node, updates list->tail using list stored in node->data.
 * 
 * Returns:
 *     - Iterator to inserted node.
 *     - On failure, returns SLLEnd.
 * 
 * Undefined Behavior:
 *     - If 'where' is NULL or SLLEnd.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
sll_iter_t SLLInsert(sll_iter_t where, void* data);

/*---------------------------------------------------------
 * Function: SLLRemove
 * --------------------------------------------------------
 * Receives:
 *     - to_remove: iterator to node to remove.
 * 
 * Does:
 *     - Removes the node and connects to the next one using data swap trick.
 *     - If removing the node before dummy, updates list->tail.
 * 
 * Returns:
 *     - Iterator to the node after the removed one.
 * 
 * Undefined Behavior:
 *     - If to_remove is NULL or SLLEnd.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
sll_iter_t SLLRemove(sll_iter_t to_remove);

/*---------------------------------------------------------
 * Function: SLLCount
 * --------------------------------------------------------
 * Receives:
 *     - list: pointer to list.
 * 
 * Does:
 *     - Counts the number of logical elements in the list.
 * 
 * Returns:
 *     - Number of elements (excluding dummy node).
 * 
 * Undefined Behavior:
 *     - If list is NULL.
 * 
 * Time Complexity:
 *     - O(n)
 ---------------------------------------------------------*/
size_t SLLCount(const sll_t* list);

/*---------------------------------------------------------
 * Function: SLLIsEmpty
 * --------------------------------------------------------
 * Receives:
 *     - list: pointer to list.
 * 
 * Does:
 *     - Checks if the list is empty.
 * 
 * Returns:
 *     - 1 if empty, 0 otherwise.
 * 
 * Undefined Behavior:
 *     - If list is NULL.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
int SLLIsEmpty(const sll_t* list);

/*---------------------------------------------------------
 * Function: SLLFind
 * --------------------------------------------------------
 * Receives:
 *     - from: iterator to start search (inclusive).
 *     - to: iterator to stop before.
 *     - is_match: match function.
 *     - param: user parameter for match function.
 * 
 * Does:
 *     - Searches for a node between [from, to) for which is_match returns true.
 * 
 * Returns:
 *     - Iterator to found node.
 *     - If not found, returns iterator to last node before SLLEnd (not necessarily 'to').
 * 
 * Undefined Behavior:
 *     - If from, to, or is_match is NULL.
 * 
 * Time Complexity:
 *     - O(n)
 ---------------------------------------------------------*/
sll_iter_t SLLFind(sll_iter_t from, sll_iter_t to, is_match_func_t is_match, void* param);

/*---------------------------------------------------------
 * Function: SLLForEach
 * --------------------------------------------------------
 * Receives:
 *     - from: iterator to start (inclusive).
 *     - to: iterator to stop before.
 *     - action_func: function to apply on each node's data.
 *     - param: user param passed to action_func.
 * 
 * Does:
 *     - Applies action_func on all nodes between [from, to).
 *     - Stops if any call returns 0.
 * 
 * Returns:
 *     - 0 if all actions succeeded.
 *     - 1 if any action_func returned 0.
 * 
 * Undefined Behavior:
 *     - If from, to, or action_func is NULL.
 * 
 * Time Complexity:
 *     - O(n)
 ---------------------------------------------------------*/
int SLLForEach(sll_iter_t from, sll_iter_t to, action_func_t action_func, void* param);

/*---------------------------------------------------------
 * Function: SLLAppend
 * --------------------------------------------------------
 * Receives:
 *     - src: source list.
 *     - dst: destination list.
 * 
 * Does:
 *     - Appends src list to dst list in constant time.
 *     - Deletes dummy of dst and reuses src nodes.
 *     - Empties src.
 * 
 * Returns:
 *     - Pointer to destination list.
 * 
 * Undefined Behavior:
 *     - If src or dst is NULL.
 * 
 * Time Complexity:
 *     - O(1)
 ---------------------------------------------------------*/
sll_t* SLLAppend(sll_t* src, sll_t* dst);

#endif /* _ILRD_SLL_ */

