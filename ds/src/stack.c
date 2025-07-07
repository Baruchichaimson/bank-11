#include <stdlib.h>  
#include <string.h>  
#include <assert.h>
#include "stack.h"

/***************************
 Exercise:  stack
 Date: 	    27/06/25
 Developer: Baruch Haimson
 Reviewer:  Bar
 Status:    
***************************/


struct Stack
{
    size_t size;
    size_t element_size;
    size_t capacity;
    char *elements;
};

stack_t* StackCreate(size_t capacity, size_t element_size)
{
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    if (!stack)
    {
        return NULL;
    }

    stack->elements = (char *)malloc(capacity * element_size);
    if (!stack->elements)
    {
        free(stack);
        return NULL;
    }

    stack->size = 0;
    stack->capacity = capacity;
    stack->element_size = element_size;

    return stack;
}

void StackDestroy(stack_t* stack)
{
    assert(stack);
    free(stack->elements);
    stack->elements = NULL;
    free(stack);
    stack = NULL;
}

void StackPush(stack_t* stack, const void* element)
{
    assert(stack);
    assert(element);
    if (stack->size < stack->capacity)
    {
    	memcpy (stack->elements + (stack->size * stack->element_size), element, stack->element_size);
    	++stack->size;
    }
}

void StackPop(stack_t* stack)
{
    assert(stack);
    if (!StackIsEmpty(stack))
	{
    	--stack->size;
    }
}

void* StackPeek(const stack_t* stack)
{
    assert(stack);
    assert(stack->size > 0);

    return (void *)(stack->elements + ((stack->size - 1) * stack->element_size));
}

size_t StackSize(const stack_t* stack)
{
    assert(stack);
    return stack->size;
}

int StackIsEmpty(const stack_t* stack)
{
    assert(stack);
    return (stack->size == 0);
}

size_t StackCapacity(const stack_t* stack)
{
    assert(stack);
    return stack->capacity;
}

