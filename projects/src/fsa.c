/************************************
 * Exercise: Fixed Size Allocator (FSA)
 * Date: 25/07/25
 * Developer: Baruch Haimson
 * Reviewer: avi
 * Status: In Progress
 ************************************/

#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */

#include "fsa.h" /* fsa API */

#define WORD_SIZE (sizeof(size_t))

#define ALIGN_UP(x)  (((x) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1))

#define MIN_BLOCK_SIZE sizeof(void*)

typedef struct FSA
{
    void* next_free;
}fsa_t;


size_t FSASuggestSize(size_t block_amount, size_t block_size)
{
	return sizeof(fsa_t) + (ALIGN_UP(block_size) * block_amount);
}

fsa_t* FSAInit(void* fsa, size_t fsa_size, size_t block_size)
{
	size_t i = 0;
    char* runner = NULL;
    
    size_t aligned_block_size = ALIGN_UP(block_size < MIN_BLOCK_SIZE ? MIN_BLOCK_SIZE : block_size);
    
    size_t num_of_blocks = (fsa_size - sizeof(fsa_t)) / aligned_block_size;

    fsa_t* pool = (fsa_t*)fsa;

    pool->next_free = (char*)pool + sizeof(fsa_t*);

    runner = (char*)pool->next_free;

    for (i = 0; i < num_of_blocks - 1; ++i)
    {
        *(void**)runner = runner + aligned_block_size;
        runner += aligned_block_size;
    }
    *(void**)runner = NULL;

    return pool;
}

void* FSAAlloc(fsa_t* fsa)
{
    assert(fsa);

    if (fsa->next_free == NULL)
    {
        return NULL; 
    }


    return;
}

void FSAFree(fsa_t* fsa, void* block)
{
    assert(fsa);
    if (block == NULL)
    {
        return;
    }

  
}

size_t FSACountFree(const fsa_t* fsa)
{
    size_t count = 0;
    void* runner = NULL;
    assert(fsa);

    runner = fsa->next_free;
    while (runner != NULL)
    {
        ++count;
        runner = *(void**)runner;
    }

    return count;
}
