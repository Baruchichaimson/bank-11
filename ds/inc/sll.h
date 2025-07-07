#ifndef _ILRD_SLL_
#define _ILRD_SLL_

typedef struct Sll sll_t;
typedef struct Node node_t;


typedef node_t* sll_iter_t;


typedef int (*is_match_func_t)(const void* data, void* param);
typedef int (*action_func_t)(void* data, void* param);

sll_t* SLLCreate(void);     /* O(1) */
void SLLDestroy(sll_t* list);   /* O(n) */
sll_iter_t SLLBegin(const sll_t* list); /* O(1) */
sll_iter_t SLLEnd(const sll_t* list);   /* O(1) */
sll_iter_t SLLNext(sll_iter_t iter);    /* O(1) */
int SLLIsEqual(sll_iter_t iter1, sll_iter_t iter2); /* O(1) */
void* SLLGetData(sll_iter_t iter);  /* O(1) */
void SLLSetData(sll_iter_t iter, void* data);   /* O(1) */
sll_iter_t SLLInsert(sll_iter_t where, void* data);     /* Insert before 'where' using data swap trick. O(1) */
sll_iter_t SLLRemove(sll_iter_t to_remove);      /* Remove and return the iter after the removed node. O(1) */
size_t SLLCount(const sll_t* list); /* O(n) */
int SLLIsEmpty(const sll_t* list);  /* O(1) */
sll_iter_t SLLFind(sll_iter_t from, sll_iter_t to, is_match_func_t is_match, void* param);    /* O(n) */
int SLLForEach(sll_iter_t from, sll_iter_t to, action_func_t action_func, void* param);   /* O(n) */
sll_t* SLLAppend(sll_t* src, sll_t* dst); /* O(1) */

#endif /* _ILRD_SLL_ */





















