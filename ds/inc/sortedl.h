#ifndef _ILRD_SORTEDL_H_
#define _ILRD_SORTEDL_H_

typedef struct sortedl_iter_t sorted_iter_t;
typedef struct sortedl sortedl_t;

struct sortedl
{
    dll_t* list;
    int (cmp*)(const void* data1, const void* data2);
};

struct sortedl_iter_t
{
    dll_iter_t iter;
    #ifndef NDEBUG
    sortedl_t* list;
    #endif /* NDEBUG */
};

sortedl_t* SortedLCreate(int(*comp)(const void* data1, const void* data2)); /* O(1) */
void SortedLDestroy(sortedl_t* list); /* O(n) */
sorted_iter_t SortedLInsert(sortedl_t* list ,void* data); /* O(n) */
sorted_iter_t SortedLRemove(sorted_iter_t to_remove); /* O(1) */
size_t SortedLSize(const sortedl_t* list); /* O(n) */
int SortedLIsEmpty(const sortedl_t* list); /* O(1) */
sorted_iter_t SortedLFind(sortedl_t* list, sorted_iter_t from, sorted_iter_t to, const void* to_find); /* O(n) */ 
sorted_iter_t SortedLFindIf(sorted_iter_t from, sorted_iter_t to, int (*is_match_func)(const void* data,const void* param), const void* param); /* O(n) */
int SortedLIsEqual(sorted_iter_t iter1, sorted_iter_t iter2); /* O(1) */
sorted_iter_t SortedLNext(sorted_iter_t curr); /* O(1) */
sorted_iter_t SortedLPrev(sorted_iter_t curr); /* O(1) */
void* SortedLGetData(sorted_iter_t iter); /* O(1) */
sorted_iter_t SortedLBegin(const sortedl_t* list); /* O(1) */
sorted_iter_t SortedLEnd(const sortedl_t* list); /* O(1) */
void* SortedLPopFront(sortedl_t* list); /* O(1) */
void* SortedLPopBack(sortedl_t* list); /* O(1) */
void SortedLMerge(sortedl_t* dest, sortedl_t* src); /* O(n) */
int SortedLForEach(sorted_iter_t from, sorted_iter_t to, int (*action_func)(void* data, void* param), void* param); /* O(n) */

#endif /* _ILRD_SORTEDL_H_ */

