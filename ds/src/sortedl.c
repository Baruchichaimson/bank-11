/***************************
 * Exercise: Sorted List
 * Date:     13/07/25
 * Developer: Baruch Haimson
 * Reviewer: Daniel
 * Status:   Approved
 ***************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "sortedl.h" /* API */
#include "dll.h" /* API */

struct sortedl
{
    dll_t* list;
    int (*cmp)(const void* data1, const void* data2);
};

typedef struct cmp
{
	int (*cmp)(const void* data1, const void* data2);
	void* data;
}cmp_h;


/*------------------------- Helper Functions -------------------------*/

static int Matchcmp(const void* data, const void* param);

/*---------------------------- API Functions -------------------------*/

sortedl_t* SortedLCreate(int (*cmp)(const void* data1, const void* data2))
{
    sortedl_t* sort_list = (sortedl_t*)malloc(sizeof(sortedl_t));
    if (!sort_list)
    {
        return NULL;
    }

    sort_list->list = DLLCreate();
    if (!sort_list->list)
    {
        free(sort_list);
        return NULL;
    }

    sort_list->cmp = cmp;

    return sort_list;
}

void SortedLDestroy(sortedl_t* list)
{
    assert(list);

    DLLDestroy(list->list);
    
    free(list);
    
    list = NULL;
}

sorted_iter_t SortedLInsert(sortedl_t* list, void* data)
{
    sorted_iter_t iter_for_insert;
    dll_iter_t pos;

	cmp_h* cmp_handler = (cmp_h*)malloc(sizeof(cmp_h));
	cmp_handler->cmp = list->cmp;
	cmp_handler->data= data;

	if(!cmp_handler)
	{
		return SortedLEnd(list);
	}

    assert(list);
    
	pos = DLLFind(DLLBegin(list->list) , DLLEnd(list->list), Matchcmp, cmp_handler);

    iter_for_insert.iter = DLLInsert(list->list, pos, data);

#ifndef NDEBUG
    iter_for_insert.list = list;
#endif

	free(cmp_handler);
	
    return iter_for_insert;
}


sorted_iter_t SortedLRemove(sorted_iter_t to_remove)
{
	assert(to_remove.iter);

    to_remove.iter = DLLRemove(to_remove.iter);

    return to_remove;
}

size_t SortedLSize(const sortedl_t* list)
{
    assert(list);

    return DLLCount(list->list);
}

int SortedLIsEmpty(const sortedl_t* list)
{
    assert(list);

    return DLLIsEmpty(list->list);
}

sorted_iter_t SortedLFind(sortedl_t* list, sorted_iter_t from, sorted_iter_t to, const void* to_find)
{
    assert(list);
    assert(from.iter);
	assert(to.iter);
	assert(from.list == to.list);

    while (!SortedLIsEqual(from, to))
    {
    	if(list->cmp(to_find, SortedLGetData(from)) == 0)
    	{
    		return from;
    	}
        from = SortedLNext(from);
    }
    
    return to;
}

sorted_iter_t SortedLFindIf(sorted_iter_t from, sorted_iter_t to, int (*is_match_func)(const void* data, const void* param), const void* param)
{
	assert(from.iter);
	assert(to.iter);
	assert(from.list == to.list);
	assert(is_match_func);
	
	from.iter = DLLFind(from.iter ,to.iter, is_match_func, param);

	return from;
}

int SortedLIsEqual(sorted_iter_t iter1, sorted_iter_t iter2)
{
	assert(iter1.list == iter2.list);

    return DLLIsEqual(iter1.iter, iter2.iter);
}

sorted_iter_t SortedLNext(sorted_iter_t curr)
{
    curr.iter = DLLNext(curr.iter);

    return curr;
}

sorted_iter_t SortedLPrev(sorted_iter_t curr)
{
    curr.iter = DLLPrev(curr.iter);

    return curr;
}

void* SortedLGetData(sorted_iter_t iter)
{
    return DLLGetData(iter.iter);
}

sorted_iter_t SortedLBegin(const sortedl_t* list)
{
    sorted_iter_t begin;
    begin.iter = DLLBegin(list->list);
#ifndef NDEBUG
    begin.list = (sortedl_t*)list;
#endif

    return begin;
}

sorted_iter_t SortedLEnd(const sortedl_t* list)
{
    sorted_iter_t end;
    end.iter = DLLEnd(list->list);
#ifndef NDEBUG
    end.list = (sortedl_t*)list;
#endif

    return end;
}

void* SortedLPopFront(sortedl_t* list)
{
    assert(list);
    assert(!SortedLIsEmpty(list));

    return DLLPopFront(list->list);
}

void* SortedLPopBack(sortedl_t* list)
{
    assert(list);
    assert(!SortedLIsEmpty(list));

    return DLLPopBack(list->list);
}

void SortedLMerge(sortedl_t* dest, sortedl_t* src)
{
	sorted_iter_t next = {0};
    sorted_iter_t from_s = SortedLBegin(src);
    sorted_iter_t to_s = SortedLEnd(src);
    
    sorted_iter_t from_d = SortedLBegin(dest);
    sorted_iter_t to_d = SortedLEnd(dest);

    while (!SortedLIsEqual(from_s, to_s))
    {
      while(!SortedLIsEqual(from_d, to_d) && dest->cmp(SortedLGetData(from_s), SortedLGetData(from_d)) > 0)
      {
      		from_d = SortedLNext(from_d);
      }
      next = SortedLNext(from_s);
      DLLSplice(from_d.iter, from_s.iter, SortedLNext(from_s).iter);
      from_s = next;
    }
}

int SortedLForEach(sorted_iter_t from, sorted_iter_t to, int (*action_func)(void* data, void* param), void* param)
{
	assert(from.list == to.list);
		
    return DLLForEach(from.iter, to.iter, action_func, param);
}

/*------------------------- Helper Functions -------------------------*/

static int Matchcmp(const void* data, const void* param)
{
	cmp_h* cmp_handler = (cmp_h*)param;
    return cmp_handler->cmp(cmp_handler->data, data) <= 0;
}


