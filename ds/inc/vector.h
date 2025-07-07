#ifndef _ILRD_VECTOR_
#define _ILRD_VECTOR_

#include <stddef.h> /* for size_t */

/***************************
 Exercise:  stack
 Date: 	    01/07/25
 Developer: Baruch Haimson
 Reviewer:  avi
 Status:    Approved
***************************/


#define DEFAULT_CAPACITY (8)

typedef struct Vector vector_t;

/* 
 * VectorCreate:
 * Creates a dynamic vector with given initial capacity and element size.
 * if capacity is less than DEFAULT_CAPACITY, it will be initialized to DEFAULT_CAPACITY.
 * Returns: pointer to the new vector, or NULL on allocation failure.
 * Undefined Behavior: if element_size == 0.
 */
vector_t* VectorCreate(size_t capacity, size_t element_size);

/* 
 * VectorDestroy:
 * Frees all memory used by the vector.
 * Input: pointer to vector.
 * If vec is NULL, behavior is safe (no-op).
 */
void VectorDestroy(vector_t* vec);

/* 
 * VectorPushBack:
 * Appends an element to the end of the vector.
 * Grows capacity if needed.
 * Returns: 0 on success, 1 on memory allocation failure.
 * Undefined Behavior: if vec or data is NULL.
 */
int VectorPushBack(vector_t* vec ,const void* data);

/* 
 * VectorPopBack:
 * Removes the last element from the vector.
 * May shrink capacity.
 * Undefined Behavior: if vec is NULL.
 */
void VectorPopBack(vector_t* vec);

/* 
 * VectorGetAccessToElement:
 * Returns a pointer to the element at the given index.
 * Undefined Behavior: if vec is NULL or index >= VectorSize(vec).
 */
void* VectorGetAccessToElement(vector_t* vec, size_t index);

/* 
 * VectorSize:
 * Returns the current number of elements in the vector.
 * Undefined Behavior: if vec is NULL.
 */
size_t VectorSize(const vector_t* vec);

/* 
 * VectorCapacity:
 * Returns the total number of elements the vector can hold before resizing.
 * Undefined Behavior: if vec is NULL.
 */
size_t VectorCapacity(const vector_t* vec);

/* 
 * VectorReserve:
 * Ensures the vector has at least the specified capacity.
 * If new_capacity < DEFAULT_CAPACITY, sets capacity to DEFAULT_CAPACITY.
 * If new_capacity <= current size, shrinks the vector to current size.
 * Returns: 0 on success, 1 on memory allocation failure.
 * Undefined Behavior: if vec is NULL.
 */
int VectorReserve(vector_t* vec, size_t new_capacity);

/* 
 * VectorShrink:
 * Shrinks the capacity of the vector to the current size.
 * If size < DEFAULT_CAPACITY, sets capacity to DEFAULT_CAPACITY.
 * Undefined Behavior: if vec is NULL.
 */
void VectorShrink(vector_t* vec);

#endif /* _ILRD_VECTOR_ */

