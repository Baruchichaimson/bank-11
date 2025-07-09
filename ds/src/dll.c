/***************************
 Exercise:  dll
 Date: 	    09/07/25
 Developer: Baruch Haimson
 Reviewer:  Lotem
 Status:    
***************************/

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "dll.h"

#define ISEND(iter) (IterToNode(iter)->next == NULL)
#define ISBEGIN(iter) (IterToNode(iter)->prev == NULL)

typedef struct node
{
    void* data;
    struct node* next;
    struct node* prev;
} node_t;

struct dll
{
    node_t head;
    node_t tail;
};

typedef enum
{
	SUCCESS = 0,
	FALIURE = -1,
	PARTIAL_FALUIRE = -2
};

/********************************** HELPER FUNCTIONS ******************************************/

static dll_iter_t NodeToIter(node_t* node);
static node_t* IterToNode(dll_iter_t iter);
static dll_iter_t CreateNode(void* data);
static int Count(void* data, void* param);
static void RemoveBetween(dll_iter_t left, dll_iter_t right, dll_iter_t remove_node);
static void InsertBetween(dll_iter_t left, dll_iter_t right, dll_iter_t new_node);

/******************************** API FUNCTIONS ******************************************/
dll_t* DLLCreate(void)
{
    dll_t* list = (dll_t*)malloc(sizeof(dll_t));
    if (!list)
    {
        return NULL;
    }

    list->head.data = NULL;
    list->head.prev = NULL;
    list->head.next = &list->tail;

    list->tail.data = NULL;
    list->tail.next = NULL;
    list->tail.prev = &list->head;

    return list;
}

void DLLDestroy(dll_t* list)
{
    
    while (!DLLIsEmpty(list))
    {
    	DLLRemove(DLLBegin(list));
    }

    free(list);
    list = NULL;
}

dll_iter_t DLLBegin(const dll_t* list)
{
    assert(list);
    return NodeToIter(list->head.next);
}

dll_iter_t DLLEnd(const dll_t* list)
{
    assert(list);
    return NodeToIter((node_t *)&list->tail);
}

dll_iter_t DLLNext(dll_iter_t iter)
{
    assert(iter);
    return NodeToIter(IterToNode(iter)->next);
}

dll_iter_t DLLPrev(dll_iter_t iter)
{
    assert(iter);
    return NodeToIter(IterToNode(iter)->prev);
}

int DLLIsEqual(dll_iter_t iter1, dll_iter_t iter2)
{
     return iter1 == iter2;
}

void* DLLGetData(dll_iter_t iter)
{
    assert(!ISEND(iter));
    assert(!ISBEGIN(iter));
    return IterToNode(iter)->data;
}

void DLLSetData(dll_iter_t iter, void* data)
{
    assert(IterToNode(iter));
    assert(!ISBEGIN(iter));
	assert(!ISEND(iter));
    IterToNode(iter)->data = data;
}

dll_iter_t DLLInsert(dll_t* list, dll_iter_t where, void* data)
{
    assert(IterToNode(where));
    
    dll_iter_t new_node = CreateNode(data);
    if (!new_node)
    {
        return DLLEnd(list);
    }

    InsertBetween(IterToNode(where)->prev, IterToNode(where), new_node);

    return NodeToIter(new_node);
}

dll_iter_t DLLRemove(dll_iter_t to_remove)
{
	node_t* to_remove_node = IterToNode(to_remove);
    node_t* next = IterToNode(to_remove_node->next);
    
	assert(to_remove_node);
	assert(!ISBEGIN(to_remove));
	assert(!ISEND(to_remove));
	
    RemoveBetween(to_remove_node->prev, to_remove_node->next, to_remove_node);
    
    free(to_remove);

    return NodeToIter(next);
}

dll_iter_t DLLPushFront(dll_t* list, void* data)
{
    return DLLInsert(list, DLLBegin(list), data);
}

dll_iter_t DLLPushBack(dll_t* list, void* data)
{
    return DLLInsert(list, DLLEnd(list), data);
}

void* DLLPopFront(dll_t* list)
{
    void* data = DLLGetData(DLLBegin(list));
    DLLRemove(DLLBegin(list));
    return data;
}

void* DLLPopBack(dll_t* list)
{
    void* data = DLLGetData(DLLPrev(DLLEnd(list)));
    DLLRemove(DLLPrev(DLLEnd(list)));
    return data;
}

size_t DLLCount(const dll_t* list)
{
    size_t counter = 0;
    DLLForEach(DLLBegin(list), DLLEnd(list), Count, &counter);
    return counter;
}

int DLLIsEmpty(const dll_t* list)
{
    return DLLIsEqual(DLLBegin(list), DLLEnd(list));
}

dll_iter_t DLLFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void*, const void*), const void* param)
{
    assert(from);
    assert(to);
    assert(is_match_func_t);

    while (!DLLIsEqual(from, to))
    {
        if ((*is_match_func_t)(DLLGetData(from), param))
        {
            return from;
        }
        from = DLLNext(from);
    }

    return to;
}

int DLLMultiFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void*, const void*), const void* param, dll_t* output)
{
	from = DLLFind(from ,to , is_match_func_t ,param);
	int amount_faliures = 0;
	
    assert(from);
    assert(to);
    assert(is_match_func_t);
    assert(output);
	
	while(!DLLIsEqual(from ,to ))
	{
		amount_faliures += DLLIsEqual(DLLPushBack(output ,DLLGetData(from)) ,DLLEnd(output));
		if (0 != amount_faliures)
		{
			return amount_faliures == 1 ? FALIURE : PARTIAL_FALUIRE;	
		}
		from = DLLNext(from);
		from = DLLFind(from ,to , is_match_func_t ,param);
	}
	return SUCCESS ;	
}

int DLLForEach(dll_iter_t from, dll_iter_t to, int (*action_func)(void*, void*), void* param)
{
    assert(from);
    assert(to);
    assert(action_func);

    while (!DLLIsEqual(from, to))
    {
        if (!action_func(DLLGetData(from), param))
        {
            return action_func(DLLGetData(from), param);
        }
        from = DLLNext(from);
    }

    return 0;
}

dll_iter_t DLLSplice(dll_iter_t where, dll_iter_t from, dll_iter_t to)
{
	node_t* where_node = IterToNode(where);
    node_t* from_node = IterToNode(from);
    node_t* to_node = IterToNode(to);

	node_t* from_prev = NULL;
    node_t* to_prev = NULL;
    node_t* where_prev = NULL;
    
    assert(where_node);
    assert(from_node);
    assert(to_node);

    if (DLLIsEqual(from, to))
    {
        return NULL;
    }
    
    from_prev = from_node->prev;
    to_prev = to_node->prev;

    /* Disconnect [from, to) */
    from_prev->next = to_node;
    to_node->prev = from_prev;

    /* Connect before where */
    where_prev = where_node->prev;
    
    where_prev->next = from_node;
    from_node->prev = where_prev;

    to_prev->next = where_node;
    where_node->prev = to_prev;

    return where;
}

/********************************** HELPER FUNCTIONS ******************************************/

static dll_iter_t NodeToIter(node_t* node)
{
	return ((dll_iter_t)node);
}

static node_t* IterToNode(dll_iter_t iter)
{
	return ((node_t*)iter);
}

static dll_iter_t CreateNode(void* data)
{
    dll_iter_t new_node = (dll_iter_t)malloc(sizeof(node_t));
    if (!new_node)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

static void InsertBetween(dll_iter_t left, dll_iter_t right, dll_iter_t new_node)
{
    new_node->prev = left;
    new_node->next = right;
    left->next = new_node;
    right->prev = new_node;
}

static void RemoveBetween(dll_iter_t left, dll_iter_t right, dll_iter_t remove_node)
{
    left->next = right;
    right->prev = left;
    remove_node->next = NULL;
    remove_node->prev = NULL;
}

static int Count(void* data, void* param)
{
    (void)data;
    ++*(size_t*)param;
    return 1;
}

