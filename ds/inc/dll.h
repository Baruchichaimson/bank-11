/***************************
 Exercise:  dll
 Date: 	    09/07/25
 Developer: Baruch Haimson
 Reviewer:  Lotem
 Status:    Approved
***************************/

#ifndef _ILRD_DLL_
#define _ILRD_DLL_

typedef struct dll dll_t;
typedef struct node node_t;
typedef node_t* dll_iter_t;

/*
Creates a new empty doubly linked list.
Parameters: None.
Returns: Pointer to the new list, or NULL on failure.
Undefined behavior: None.
Time complexity: O(1).
*/
dll_t* DLLCreate(void);

/*
Destroys the list and frees all memory.
Parameters: list - pointer to the list.
Returns: void.
Undefined behavior: if list is NULL.
Time complexity: O(n).
*/
void DLLDestroy(dll_t* list);

/*
Returns an iterator to the first data node in the list.
Parameters: list - pointer to the list.
Returns: iterator to the first node.
Undefined behavior: if list is NULL.
Time complexity: O(1).
*/
dll_iter_t DLLBegin(const dll_t* list);

/*
Returns an iterator to the end (tail dummy node) of the list.
Parameters: list - pointer to the list.
Returns: iterator to the end node.
Undefined behavior: if list is NULL.
Time complexity: O(1).
*/
dll_iter_t DLLEnd(const dll_t* list);

/*
Returns the next iterator after the given iterator.
Parameters: curr - current iterator.
Returns: next iterator.
Undefined behavior: if curr is NULL.
Time complexity: O(1).
*/
dll_iter_t DLLNext(dll_iter_t curr);

/*
Returns the previous iterator before the given iterator.
Parameters: curr - current iterator.
Returns: previous iterator.
Undefined behavior: if curr is NULL.
Time complexity: O(1).
*/
dll_iter_t DLLPrev(dll_iter_t curr);

/*
Checks if two iterators are equal.
Parameters: iter1, iter2 - the iterators to compare.
Returns: 1 if equal, 0 otherwise.
Undefined behavior: None.
Time complexity: O(1).
*/
int DLLIsEqual(dll_iter_t iter1, dll_iter_t iter2);

/*
Returns the data stored at the given iterator.
Parameters: iter - the iterator.
Returns: pointer to data.
Undefined behavior: if iter is NULL or points to end.
Time complexity: O(1).
*/
void* DLLGetData(dll_iter_t iter);

/*
Sets the data at the given iterator.
Parameters: iter - the iterator, data - new data to set.
Returns: void.
Undefined behavior: if iter is NULL or points to end.
Time complexity: O(1).
*/
void DLLSetData(dll_iter_t iter, void* data);

/*
Inserts a new node before the given iterator.
Parameters: list - the list, where - iterator before which to insert, data - data to insert.
Returns: iterator to the new node or DLLEnd on failure.
Undefined behavior: if 'where' is not in 'list'.
Time complexity: O(1).
*/
dll_iter_t DLLInsert(dll_t* list, dll_iter_t where, void* data);

/*
Removes the node at the given iterator.
Parameters: to_remove - iterator to the node to remove.
Returns: iterator to the node after the removed node.
Undefined behavior: if to_remove is end or NULL.
Time complexity: O(1).
*/
dll_iter_t DLLRemove(dll_iter_t to_remove);

/*
Pushes a new node to the front of the list.
Parameters: list - pointer to the list, data - data to insert.
Returns: iterator to the new node.
Undefined behavior: if list is NULL.
Time complexity: O(1).
*/
dll_iter_t DLLPushFront(dll_t* list, void* data);

/*
Pushes a new node to the back of the list.
Parameters: list - pointer to the list, data - data to insert.
Returns: iterator to the new node.
Undefined behavior: if list is NULL.
Time complexity: O(1).
*/
dll_iter_t DLLPushBack(dll_t* list, void* data);

/*
Removes the front node from the list.
Parameters: list - pointer to the list.
Returns: data from the removed node.
Undefined behavior: if list is empty.
Time complexity: O(1).
*/
void *DLLPopFront(dll_t* list);

/*
Removes the back node from the list.
Parameters: list - pointer to the list.
Returns: data from the removed node.
Undefined behavior: if list is empty.
Time complexity: O(1).
*/
void *DLLPopBack(dll_t* list);

/*
Counts the number of elements in the list.
Parameters: list - pointer to the list.
Returns: number of data nodes.
Undefined behavior: if list is NULL.
Time complexity: O(n).
*/
size_t DLLCount(const dll_t* list);

/*
Checks if the list is empty.
Parameters: list - pointer to the list.
Returns: 1 if empty, 0 otherwise.
Undefined behavior: if list is NULL.
Time complexity: O(1).
*/
int DLLIsEmpty(const dll_t* list);

/*
Finds the first node in [from, to) range for which is_match_func returns true.
Parameters: from - starting iterator, to - ending iterator, is_match_func_t - match function, param - user parameter.
Returns: iterator to matching node, or 'to' if none found.
Undefined behavior: if from, to, or is_match_func_t is NULL.
Time complexity: O(n).
*/
dll_iter_t DLLFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void* data, const void* param), const void* param);

/*
Finds all nodes in [from, to) matching the match function and appends them to output list.
Parameters: from, to - range of iterators, is_match_func_t - match function, param - user data, output - list to store results.
Returns: SUCCESS, FALIURE, or PARTIAL_FAILURE.
Undefined behavior: if any parameter is NULL.
Time complexity: O(n).
*/
int DLLMultiFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void* data, const void* param), const void* param, dll_t* output);

/*
Applies action_func to all elements in range [from, to).
Parameters: from, to - range of iterators, action_func_t - function to apply, param - user parameter.
Returns: 0 if successful, non-zero on failure.
Undefined behavior: if from, to, or action_func_t is NULL.
Time complexity: O(n).
*/
int DLLForEach(dll_iter_t from, dll_iter_t to, int (*action_func_t)(void* data, void* param), void* param);

/*
Splices (moves) the range [from, to) before 'where' in constant time.
Parameters: where - target insert location, from - start of range, to - end of range (exclusive).
Returns: iterator to where.
Undefined behavior: if iterators do not belong to valid lists or from == to.
Time complexity: O(1).
*/
dll_iter_t DLLSplice(dll_iter_t where, dll_iter_t from, dll_iter_t to);

#endif /* _ILRD_DLL_ */
