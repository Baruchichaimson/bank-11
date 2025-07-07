#ifndef _ILRD_STACK_
#define _ILRD_STACK_

#include <stddef.h> /* size_t */

/*------------------------------------------------------------
    STACK ADT

    Description:
        Generic stack data structure with fixed capacity.
        Supports push, pop, peek, and query operations.
    Complexity:
        All operations are O(1) time.
-------------------------------------------------------------*/

typedef struct Stack stack_t;

/*------------------------------------------------------------
    StackCreate

    Description:   Allocates and initializes a new stack with given capacity
                   and size of each element in bytes.

    Arguments:     capacity      - Maximum number of elements the stack can hold
                   element_size  - Size of each element in bytes

    Return:         Pointer to the new stack on success, or NULL on failure
-------------------------------------------------------------*/

stack_t* StackCreate(size_t capacity, size_t element_size);
/*------------------------------------------------------------
    StackDestroy

    Description:  Frees all resources used by the stack.
    Arguments:    stack - Pointer to the stack
-------------------------------------------------------------*/
void StackDestroy(stack_t* stack);

/*------------------------------------------------------------
    StackPush

    Description:  Pushes a new element to the top of the stack 
    			  if the stack is not full.
 
    Arguments:    stack   - Pointer to the stack
                  element - Pointer to the data to push
-------------------------------------------------------------*/
void StackPush(stack_t* stack, const void* element);

/*------------------------------------------------------------
    StackPop

    Description:  Removes the top element from the stack if the stack is not empty.
    Arguments:    stack - Pointer to the stack
-------------------------------------------------------------*/
void StackPop(stack_t* stack);

/*------------------------------------------------------------
    StackPeek

    Description:  Returns a pointer to the top element without removing it if is exsits.
    Arguments:    stack - Pointer to the stack
    Return:       Pointer to the top element
-------------------------------------------------------------*/
void* StackPeek(const stack_t* stack);

/*------------------------------------------------------------
    StackSize

    Description:  Returns the number of elements currently in the stack.
    Arguments:    stack - Pointer to the stack
    Return:       Number of elements
-------------------------------------------------------------*/
size_t StackSize(const stack_t* stack);

/*
    StackIsEmpty

    Description: Checks whether the stack is empty.
    Arguments:   stack - Pointer to the stack
    Return:      1 if empty, 0 otherwise
*/
int StackIsEmpty(const stack_t* stack);

/*
    StackCapacity

    Description:  Returns the total capacity of the stack.
    Arguments:    stack - Pointer to the stack
    Return:       Maximum number of elements the stack can hold
*/
size_t StackCapacity(const stack_t* stack);

#endif /* _ILRD_STACK_ */

