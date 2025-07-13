/***************************
 * Exercise: Sorted List
 * Date:     10/07/25
 * Developer: Baruch Haimson
 * Reviewer: 
 * Status:   In Progress
 ***************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "sortedl.h"
#include "dll.h"

struct sortedl
{
    dll_t* list;
    int (*cmp)(const void* data1, const void* data2);
};

/*------------------------- Helper Functions -------------------------*/

static dll_iter_t FindInsertPosition(const sortedl_t* list, const void* data);

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

    assert(list);

    dll_iter_t pos = FindInsertPosition(list, data);

    iter_for_insert.iter = DLLInsert(list->list, pos, data);

#ifndef NDEBUG
    iter_for_insert.list = list;
#endif

    return iter_for_insert;
}


sorted_iter_t SortedLRemove(sorted_iter_t to_remove)
{
    sorted_iter_t next;

    next.iter = DLLNext(to_remove.iter);
#ifndef NDEBUG
    next.list = to_remove.list;
#endif

    DLLRemove(to_remove.iter);

    return next;
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
    	if(list->cmp(to_find, SortedLGetData(from)) <= 0)
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
	
	sorted_iter_t result;
	
	result.iter = DLLFind(from.iter ,to.iter, is_match_func, param);
#ifndef NDEBUG
    result.list = result.list;
#endif
	return result;
}

int SortedLIsEqual(sorted_iter_t iter1, sorted_iter_t iter2)
{
    return DLLIsEqual(iter1.iter, iter2.iter);
}

sorted_iter_t SortedLNext(sorted_iter_t curr)
{
    sorted_iter_t next;
    next.iter = DLLNext(curr.iter);
#ifndef NDEBUG
    next.list = curr.list;
#endif

    return next;
}

sorted_iter_t SortedLPrev(sorted_iter_t curr)
{
    sorted_iter_t prev;
    prev.iter = DLLPrev(curr.iter);
#ifndef NDEBUG
    prev.list = curr.list;
#endif

    return prev;
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

    return DLLPopFront(list->list);
}

void* SortedLPopBack(sortedl_t* list)
{
    assert(list);

    return DLLPopBack(list->list);
}

void SortedLMerge(sortedl_t* dest, sortedl_t* src)
{
    sorted_iter_t from = SortedLBegin(src);
    sorted_iter_t to = SortedLEnd(src);

    while (!SortedLIsEqual(from, to))
    {
        sorted_iter_t next = SortedLNext(from);
        void* data = SortedLGetData(from);

        SortedLInsert(dest, data);
        SortedLRemove(from);

        from = next;
    }
}

int SortedLForEach(sorted_iter_t from, sorted_iter_t to, int (*action_func)(void* data, void* param), void* param)
{
    return DLLForEach(from.iter, to.iter, action_func, param);
}

/*------------------------- Helper Functions -------------------------*/

static dll_iter_t FindInsertPosition(const sortedl_t* list, const void* data)
{
	dll_iter_t iter = DLLBegin(list->list);

    while (!DLLIsEqual(iter, DLLEnd(list->list)) && list->cmp(data, DLLGetData(iter)) > 0)
    {
        iter = DLLNext(iter);
    }

    return iter;
}



