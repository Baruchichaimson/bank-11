/***************************
 * Exercise: Sorted List
 * Date:     13/07/25
 * Developer: Baruch Haimson
 * Reviewer: Daniel
 * Status:   Approved
 ***************************/

#ifndef _ILRD_SORTEDL_H_
#define _ILRD_SORTEDL_H_

#include <stddef.h>
#include "dll.h"

typedef struct sortedl_iter_t sorted_iter_t;
typedef struct sortedl sortedl_t;

struct sortedl_iter_t
{
    dll_iter_t iter;
    #ifndef NDEBUG
    sortedl_t* list;
    #endif /* NDEBUG */
};

/**
 * Description: Creates a new Sorted List.
 *
 * Function Arguments :
 *		 comp - compare function to use for sorting. 
 *
 * Return Value:
 *        - A pointer to a newly allocated sorted list.
 *        - NULL if memory allocation failed.
 *
 * General notes: None.
 *
 * Complexity:
 *        - Time: O(1).
 **/
sortedl_t* SortedLCreate(int(*comp)(const void* data1, const void* data2));

/**
 * Description: Free all memory allocated for list and its elements.
 *
 * Function Arguments : 
 *		 list - Sorted list to be freed. 
 *
 * Return Value: None.
 *
 * General notes:
 *        - Behavior is undefined if list pointer is NULL.
 *        - Behavior is undefined if container pointer is NULL.
 *
 * Complexity:
 *        - Time: O(n).
 **/
void SortedLDestroy(sortedl_t* list);

/**
 * Description: Insert a new element to the list.
 *
 * Function Arguments :
 *		 list - list to insert the element to.  
 *		 data - element data to insert.  
 *
 * Return Value:
 *        - iterator to the new element inserted.
 *
 * General notes:
 *		  - Insertion happens before where iter.			
 *        - Behavior is undefined if list pointer is NULL.
 *        - Behavior is undefined if container pointer is NULL.
 *
 * Complexity:
 *        - Time: O(n).
 **/
sorted_iter_t SortedLInsert(sortedl_t* list ,void* data);

/**
 * Description: Remove an element from the list.
 *
 * Function Arguments : 
 *		 to_remove - iterator to element than needs to be removed. 
 *
 * Return Value:
 *        - iterator to the element after the one taken out of the list.
 *
 * General notes:
 *        - Behavior is undefined if iter pointer is NULL.
 *        - Behavior is undefined if iter pointer is head.
 *        - Behavior is undefined if iter pointer is tail.
 *
 * Complexity:
 *        - Time: O(1).
 **/
sorted_iter_t SortedLRemove(sorted_iter_t to_remove);

/**
 * Description: Countes the number of element in the list.
 *
 * Function Arguments : 
 *		 list - list to count elements. 
 *
 * Return Value:
 *        - size of given list.
 *
 * General notes:
 *        - Behavior is undefined if list pointer in NULL.
 *
 * Complexity:
 *        - Time: O(n).
 **/
size_t SortedLSize(const sortedl_t* list);

/**
 * Description: Check if a given list is empty.
 *
 * Function Arguments : 
 *		 list - list to check. 
 *
 * Return Value:
 *        - 1 if list is empty.
 *        - 0 if not.
 *
 * General notes:
 *        - Behavior is undefined if list pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
int SortedLIsEmpty(const sortedl_t* list);

/**
 * Description: Returns an iterator to the element that matches the param using the lists compare function.
 *
 * Function Arguments : 
 *       list - list to search the element in.
 *       from - iterator to start searching from.
 *       to - iterator to stop searching.
 *       to_find - element value to find a match to.
 *
 * Return Value:
 *        - iterator to the first matching element.
 *        - iterator "to" if no element was found.
 *
 * General notes:
 *        - Behavior is undefined if list pointer is NULL.
 *        - Behavior is undefined if "from" iterator is NULL.
 *        - Behavior is undefined if "to" iterator is NULL.
 *        - Behavior is undefined if ""from" and "to" are not from the same list.
 *
 * Complexity:
 *        - Time: O(n).
 **/
sorted_iter_t SortedLFind(sortedl_t* list, sorted_iter_t from, sorted_iter_t to, const void* to_find);

/**
 * Description: Returns an iterator to the element that matches the param using the given compare function.
 *
 * Function Arguments : 
 *       is_match_func - Compare function.
 *       from - iterator to start searching from.
 *       to - iterator to stop searching.
 *       param - element value to find a match to.
 *
 * Return Value:
 *        - iterator to the first matching element.
 *        - iterator "to" if no element was found.
 *
 * General notes:
 *        - Behavior is undefined if is_match_func pointer is NULL.
 *        - Behavior is undefined if "from" iterator is NULL.
 *        - Behavior is undefined if "to" iterator is NULL.
 *        - Behavior is undefined if ""from" and "to" are not from the same list.
 *
 * Complexity:
 *        - Time: O(n).
 **/
sorted_iter_t SortedLFindIf(sorted_iter_t from, sorted_iter_t to, int (*is_match_func)(const void* data,const void* param), const void* param);

/**
 * Description: Check if two iterator are the same.
 *
 * Function Arguments :
 *		 iter1 - first iter to compare.
 *		 iter2 - second iter to compare. 
 *
 * Return Value:
 *        - 1 if iterators are equal.
 *        - 0 otherwise.
 *
 * General notes:
 *        - Behavior is undefined if either iterator is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
int SortedLIsEqual(sorted_iter_t iter1, sorted_iter_t iter2);

/**
 * Description: Returns an iter to the next element.
 *
 * Function Arguments :
 *		 curr - iter to the current element.  
 *
 * Return Value:
 *        - An iter to the next element.
 *
 * General notes:
 *        - Behavior is undefined if curr pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
sorted_iter_t SortedLNext(sorted_iter_t curr);

/**
 * Description: Returns an iter to the previous element.
 *
 * Function Arguments :
 *		 curr - iter to the current element.  
 *
 * Return Value:
 *        - An iter to the previous element.
 *
 * General notes:
 *        - Behavior is undefined if curr pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
sorted_iter_t SortedLPrev(sorted_iter_t curr);

/**
 * Description: Returns the data of the given iter.
 *
 * Function Arguments : 
 *		 iter - iter whos data is requested.  
 *
 * Return Value:
 *        - given iters data.
 *
 * General notes:
 *        - Behavior is undefined if iter pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
void* SortedLGetData(sorted_iter_t iter);

/**
 * Description: Returns an iter to the first element in the list.
 *
 * Function Arguments :
 *		 list - sorted list wanted.  
 *
 * Return Value:
 *        - An iter to the first element.
 *
 * General notes:
 *        - Behavior is undefined if list pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
sorted_iter_t SortedLBegin(const sortedl_t* list);

/**
 * Description: Returns an iter to the dummy element in the end of the list.
 *
 * Function Arguments : 
 *		 list - sorted list wanted.  
 *
 * Return Value:
 *        - An iter to the dummy element.
 *
 * General notes:
 *        - Behavior is undefined if list pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
sorted_iter_t SortedLEnd(const sortedl_t* list);

/**
 * Description: Remove an element from the beginning of the list.
 *
 * Function Arguments :
 *		 list - list to remove element from.
 *
 * Return Value:
 *        - Data of element removed.
 *
 * General notes:
 *        - Behavior is undefined if list pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
void* SortedLPopFront(sortedl_t* list);

/**
 * Description: Remove an element from the end of the list.
 *
 * Function Arguments : 
 *		 list - list to remove element from.
 *
 * Return Value:
 *        - Data of element removed.
 *
 * General notes:
 *        - Behavior is undefined if list pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
void* SortedLPopBack(sortedl_t* list); 

/**
 * Description: Merges two sorted lists.
 *
 * Function Arguments :
 *       dest - destination list.
 *       src - source list.
 *
 * Return Value: None.
 *
 * Notes:
 *        - Behavior is undefined if either lists pointers are NULL.
 *        - Behavior is undefined if either lists container is NULL.
 *        - Behavior is undefined if both lists have different compare functions.
 *
 * Complexity:
 *       - Time: O(n).
 */
void SortedLMerge(sortedl_t* dest, sortedl_t* src);

/**
 * Description: Preforms an action on each element in a given range.
 *
 * Function Arguments :
 *       from - iterator to start from.
 *       to - iterator to stop.
 *       action_func - function that preforms requiered action.
 *       param - parameter to pass to action function.
 *
 * Return Value:
 *        - 0 for SUCCESS.
 *        - other for FAILIOR.
 *
 * Notes:
 *        - Behavior is undefined if "from" iterator is NULL.
 *        - Behavior is undefined if "to" iterator is NULL.
 *        - Behavior is undefined if "action_func_t" pointer is NULL.
 *
 * Complexity:
 *       - Time: O(n).
 */
int SortedLForEach(sorted_iter_t from, sorted_iter_t to, int (*action_func)(void* data, void* param), void* param);

#endif /* _ILRD_SORTEDL_H_ */

