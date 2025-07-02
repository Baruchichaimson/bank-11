#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "sll.h"

struct Node
{
    void* data;
    node_t* next;
};

struct Sll
{
    node_t* head;
    node_t* tail;
};


sll_t* SLLCreate(void)
{
	sll_t* list = (sll_t*)malloc(sizeof(sll_t));
    if (!list)
    {
        return NULL;
    }

    list->head = (node_t*)malloc(sizeof(node_t));
    if (!list->head)
    {
        free(list);
        return NULL;
    }

    list->head->data = list;       
    list->head->next = NULL;

    list->tail = list->head;       
    return list;
}

void SLLDestroy(sll_t* list)
{
	
	node_t* currentNode = list->head;
	node_t* node = NULL;
	if(list == NULL)
	{
		return;
	}
	
	while(currentNode != list->tail)
	{
		node = currentNode->next;
		free(currentNode);
		currentNode = node;
	}
	free(currentNode); /* tail */
	free(list);
	list = NULL;
}

sll_iter_t SLLBegin(const sll_t* list)
{
	return list->head;
}

sll_iter_t SLLEnd(const sll_t* list)
{
	return list->tail;
}

sll_iter_t SLLNext(sll_iter_t iter)
{
	return iter->next;
}

int SLLIsEqual(sll_iter_t iter1, sll_iter_t iter2)
{
	return (iter1 == iter2);
}

void* SLLGetData(sll_iter_t iter)
{
	return iter->data;
}

void SLLSetData(sll_iter_t iter, void* data)
{
	iter->data = data;
}

sll_iter_t SLLInsert(sll_iter_t where, void* data)
{
	sll_t* list = NULL;
	node_t* node = (node_t*)malloc(sizeof(node_t));
	if(!node)
	{
		return SLLEnd((sll_t*)where->data);
	}
	node->data = where->data;
	node->next = where->next;
	where->data = data;
	where->next = node;

	if (node->next == NULL)
    {
    	list = (sll_t*)node->data;
    	list->tail = node;
    }
	return where;
}
sll_iter_t SLLRemove(sll_iter_t to_remove)
{
	node_t* node = NULL;
	sll_iter_t next = NULL;
	assert(to_remove);
	assert(to_remove->next);

	node = to_remove->next;
	to_remove->data = node->data;
	to_remove->next = node->next;
	
	next = node->next;
    free(node);
    return next;
}
size_t SLLCount(const sll_t* list)
{
	size_t count = 0; 
	node_t* currentNode = list->head;
	assert(list);
	
	while(currentNode != list->tail)
	{
		count++;
		currentNode = currentNode->next;
	}
	return count;
}
int SLLIsEmpty(const sll_t* list)
{
	return (list->head == list->tail);
}

sll_iter_t SLLFind(sll_iter_t from, sll_iter_t to, is_match_func_t is_match, void* param)
{
	assert(is_match);
	while(!SLLIsEqual(from, to))
	{
		if(is_match(from->data, param))
		{
			return from;
		}
		from = from->next;
	}
	return to;
}
int SLLForEach(sll_iter_t from, sll_iter_t to, action_func_t action_func, void* param)
{
	assert(action_func);
	while(!SLLIsEqual(from, to))
	{
		if(!action_func(from->data, param))
		{
			return 1;
		}
		from = from->next;
	}
	return 0;
}









