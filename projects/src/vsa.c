#include <assert.h> /* assert */

#include "vsa.h" /* API vsa */

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define WORD_SIZE sizeof(size_t)
#define ALIGN_UP(x)  (((size_t)(x) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1))
#define ALIGN_DOWN(x)  (((size_t)(x)) & ~(WORD_SIZE - 1))

struct VSA 
{
	void* end_pool;
};

typedef struct Header
{
	long block_size;
	#ifndef NDEBUG
		size_t magic_number;	
	#endif
}header_t;

/********************** helper function *********************************************/
static void checkRemain(header_t* curr, size_t aligned_size);
static header_t* freeNeighbors(vsa_t* vsa, size_t aligned_size);
/************************************************************************************/

vsa_t* VSAInit(void* pool, size_t pool_size)
{
	vsa_t* vsa = (vsa_t*)ALIGN_UP(pool);
	size_t aligned_size = ALIGN_DOWN(pool_size - ((char*)vsa - (char*)pool));
	
	header_t* header = (header_t*)((char*)vsa + sizeof(vsa_t));
	size_t header_space = aligned_size - sizeof(vsa_t);
	
	vsa->end_pool = (char*)vsa + aligned_size;

	header->block_size = header_space - sizeof(header_t);
	#ifndef NDEBUG
		header->magic_number = 0;
	#endif
	
	return vsa;
}

void* VSAAlloc(vsa_t* vsa, size_t block_size)
{
	header_t* curr = NULL;
	
	size_t aligned_size = ALIGN_UP(block_size);

	assert(vsa);

	curr = freeNeighbors(vsa, aligned_size);
	if(!curr)
	{
		return NULL;
	}

	checkRemain(curr, aligned_size);
	
	#ifndef NDEBUG
		curr->magic_number = 0xDEADBEEF;
	#endif

	return (char*)curr + sizeof(header_t);

}

void VSAFree(void* block)
{
	header_t* header = NULL;

    if (block == NULL)
    {
        return;
    }

    header = (header_t*)((char*)block - sizeof(header_t));

	assert(header->magic_number == 0xDEADBEEF);

    header->block_size = -header->block_size;
	#ifndef NDEBUG
		header->magic_number = 0;
	#endif
}

size_t VSALargestChunkAvailable(vsa_t* vsa)
{
	header_t* next = NULL;
	size_t largest = 0;
    header_t* curr = (header_t*)((char*)vsa + sizeof(vsa_t));
    
	assert(vsa);
	
    while ((char*)curr < (char*)vsa->end_pool)
    {
        if (curr->block_size > 0)
        {
            next = (header_t*)((char*)curr + sizeof(header_t) + curr->block_size);

            while ((char*)next < (char*)vsa->end_pool && next->block_size > 0)
            {
                curr->block_size += sizeof(header_t) + next->block_size;
                
                next = (header_t*)((char*)next + sizeof(header_t) + next->block_size);
            }
			if((size_t)curr->block_size > largest)
			{
				largest = curr->block_size;
			}
        }

        curr = (header_t*)((char*)curr + sizeof(header_t) + ABS(curr->block_size));
    }
	return largest;
}

/********************** helper function ******************************************************************/

static void checkRemain(header_t* curr, size_t aligned_size)
{
	header_t* new_block = NULL;
	size_t new_block_size = 0;
	long remaining = 0;

	if (curr->block_size  < 0)
    {
        return;
    }
	
	remaining = curr->block_size - (long)aligned_size;

	if (remaining >= (long)(sizeof(header_t) + sizeof(size_t)))
    {
        new_block = (header_t*)((char*)curr + sizeof(header_t) + aligned_size);

		new_block_size = (size_t)(remaining - sizeof(header_t));
		new_block_size -= new_block_size % sizeof(size_t); 
		
        new_block->block_size = (long)new_block_size;
        
        curr->block_size = -(long)aligned_size;
    }
    else
    {
        curr->block_size = -curr->block_size;
    }
	
}

static header_t* freeNeighbors(vsa_t* vsa, size_t aligned_size)
{
	header_t* next = NULL;
    header_t* curr = (header_t*)((char*)vsa + sizeof(vsa_t));
    
	assert(vsa);
	
    while ((char*)curr < (char*)vsa->end_pool)
    {
        if (curr->block_size > 0)
        {
            next = (header_t*)((char*)curr + sizeof(header_t) + curr->block_size);

            while ((char*)next < (char*)vsa->end_pool && next->block_size > 0 && curr->block_size < (long)aligned_size)
            {
                curr->block_size += sizeof(header_t) + next->block_size;

                next = (header_t*)((char*)next + sizeof(header_t) + next->block_size);
            }
            
			if (curr->block_size >= (long)aligned_size)
            {
                return curr; 
            }
        }

        curr = (header_t*)((char*)curr + sizeof(header_t) + ABS(curr->block_size));
    }
	return NULL;
}
