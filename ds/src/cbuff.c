#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "cbuff.h"

/***************************
 Exercise:  cbuff
 Date: 	    07/07/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    Approved
***************************/



struct cbuff
{
    size_t front;
    size_t size;
    size_t capacity;
    char buffer[1];
};



cbuff_t* CBuffCreate(size_t capacity)
{
	cbuff_t* cbuff = (cbuff_t*)malloc(sizeof(cbuff) + capacity * sizeof(char));
	if(!cbuff)
	{
		return NULL;
	}
	cbuff->front = 0;
    cbuff->size = 0;
    cbuff->capacity = capacity;
    return cbuff;
}

void CBuffDestroy(cbuff_t* cbuff)
{
	assert(cbuff);
	free(cbuff);
	cbuff = NULL;
}


ssize_t CBuffWrite(cbuff_t* cbuff, const void* src, size_t bytes)
{
	assert (cbuff && src); 
	size_t amount_write = (bytes < CBuffFreeSpace(cbuff)) ? bytes : CBuffFreeSpace(cbuff);
	
    size_t start = (cbuff->front + cbuff->size) % cbuff->capacity;
    
    size_t space_in_end = 0;
    
    if (src == NULL) 
    {
    	return -1;
    }
    
    if (start + amount_write <= cbuff->capacity)
    {
        memcpy(cbuff->buffer + start, src, amount_write);
    }
    else
    {
        space_in_end = cbuff->capacity - start;
        memcpy(cbuff->buffer + start, src, space_in_end);
        memcpy(cbuff->buffer, (char *)src + space_in_end, amount_write - space_in_end);
    }

    cbuff->size += amount_write;
    return (ssize_t)amount_write;
}


ssize_t CBuffRead(cbuff_t* cbuff, void* dst, size_t bytes)
{
    assert(cbuff && dst);
	
	size_t amount_read = (bytes < cbuff->size) ? bytes : cbuff->size;
    size_t space_in_end = 0;
    
    if (dst == NULL) 
    {
    	return -1;
	}
	
    if (cbuff->front + amount_read <= cbuff->capacity)
    {
        memcpy(dst, cbuff->buffer + cbuff->front, amount_read);
    }
    else
    {
        space_in_end = cbuff->capacity - cbuff->front;
        memcpy(dst, cbuff->buffer + cbuff->front, space_in_end);
        memcpy((char *)dst + space_in_end, cbuff->buffer, amount_read - space_in_end);
    }

    cbuff->front = (cbuff->front + amount_read) % cbuff->capacity;
    
    cbuff->size -= amount_read;
    
    return (ssize_t)amount_read;
}

int CBuffIsEmpty(cbuff_t* cbuff)
{
	assert(cbuff);
	return (cbuff->size == 0);
}


size_t CBuffFreeSpace(cbuff_t* cbuff)
{
	assert(cbuff);
	return (cbuff->capacity - cbuff->size);
}

size_t CBuffSize(cbuff_t* cbuff)
{
	assert(cbuff);
	return cbuff->size;
}


