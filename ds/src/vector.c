#include <stdlib.h>     /* malloc, realloc, free */
#include <string.h>     /* memcpy */
#include <assert.h>     /* assert */
#include "vector.h"     /* vector_t interface */

/***************************
 Exercise:  stack
 Date: 	    01/07/25
 Developer: Baruch Haimson
 Reviewer:  avi
 Status:    Approved
***************************/

struct Vector
{
	char* buffer;
	size_t element_size;
	size_t capacity;
	size_t size;

};

vector_t* VectorCreate(size_t init_capacity, size_t element_size)
{
	if (element_size == 0)
    {
        return NULL;
    }
    
    vector_t* vec = (vector_t*)malloc(sizeof(vector_t));
    if (!vec)
    { 
    	return NULL;
    }
	vec->capacity = init_capacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : init_capacity;
    vec->buffer = (char*)malloc(vec->capacity * element_size);
    if (!vec->buffer)
    {
        free(vec);
        return NULL;
    }
    
	vec->size = 0;
    vec->element_size = element_size;

   
    return vec;
}

void VectorDestroy(vector_t* vec)
{  
    free(vec->buffer);
    vec->buffer = NULL;
    free(vec);
    vec = NULL;
}


int VectorPushBack(vector_t* vec, const void* data)
{
    assert(vec); 
    assert(data);

    if (vec->size == vec->capacity)
    {
        if (VectorReserve(vec, vec->capacity * 2) != 0)
        {
            return 1; 
        }
    }
    memcpy(vec->buffer + (vec->size * vec->element_size), data, vec->element_size);
    ++vec->size;

    return 0;
}


void VectorPopBack(vector_t* vec)
{
    assert(vec);
    assert(vec->size > 0);
    --vec->size;
    if (vec->size * 4 <= vec->capacity)
    {
        VectorReserve(vec, vec->capacity / 2);
    }
}

void* VectorGetAccessToElement(vector_t* vec, size_t index)
{
    assert(vec);
    assert(index < vec->size);
    return (void*)(vec->buffer + (index * vec->element_size));
}

size_t VectorSize(const vector_t* vec)
{
    assert(vec);
    return vec->size;
}


size_t VectorCapacity(const vector_t* vec)
{
    assert(vec);
    return vec->capacity;
}


int VectorReserve(vector_t* vec, size_t new_capacity)
{
	char* new_buffer = NULL;
    assert(vec);
	if(new_capacity < DEFAULT_CAPACITY)
	{
		new_capacity = DEFAULT_CAPACITY;
	}
    else if (new_capacity <= vec->size)
    {
        VectorShrink(vec);
        return 0;
    }

    new_buffer = (char*)realloc(vec->buffer, new_capacity * vec->element_size);
    if (!new_buffer)
    {
    	VectorDestroy(vec);
        return 1; 
    }
    vec->buffer = new_buffer;
    vec->capacity = new_capacity;
    return 0;
}


void VectorShrink(vector_t* vec)
{
	char* new_buffer;
    assert(vec);
	new_buffer = (char*)realloc(vec->buffer, vec->size * vec->element_size);
    
	vec->capacity = vec->size;
	if(vec->capacity < DEFAULT_CAPACITY)
	{
		vec->capacity = DEFAULT_CAPACITY;
	}
	vec->buffer = new_buffer;
}

