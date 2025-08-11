#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define POOL_SIZE 2000

#include "vsa.h"

typedef struct VSA 
{
	void* end_pool;
}vsa_tt;

typedef struct Header
{
	long block_size;
	#ifndef NDEBUG
		size_t magic_number;	
	#endif
}header_t;

#define ABS(x) ((x) < 0 ? -(x) : (x))

int CountHeaders(vsa_t* vsa)
{
    header_t* curr = (header_t*)((char*)vsa + sizeof(vsa_tt));
    int count = 0;

    while ((char*)curr < (char*)vsa->end_pool)
    {
        if (curr->block_size == 0)
        {
            break;
        }
        ++count;
        curr = (header_t*)((char*)curr + sizeof(header_t) + ABS(curr->block_size));
    }

    return count;
}

void TestVSAInit()
{
    size_t largest = 0;
    char pool[POOL_SIZE] = {0};
    vsa_t* vsa = VSAInit(pool, POOL_SIZE);

    assert(vsa != NULL);

    largest = VSALargestChunkAvailable(vsa);
    assert(largest > 0 && largest <= POOL_SIZE);

    printf("TestVSAInit passed\n");
}

void TestVSAAlloc()
{
    void* p1 = NULL;
    void* p2 = NULL;
    void* p3 = NULL;
    void* p4 = NULL;
    void* p5 = NULL;
    int headers_after = 0;
    int headers_before = 0;
	char pool[POOL_SIZE] = {0};
    vsa_t* vsa = VSAInit(pool, POOL_SIZE);

    p1 = VSAAlloc(vsa, 50);
    assert(p1 != NULL);

    p2 = VSAAlloc(vsa, 100);
    assert(p2 != NULL);

    p3 = VSAAlloc(vsa, 30);
    assert(p3 != NULL);

    p4 = VSAAlloc(vsa, 20);
    assert(p4 != NULL);

    headers_before = CountHeaders(vsa);
    printf("Headers count before p5 allocation: %d\n", headers_before);

    VSAFree(p1);
    VSAFree(p2);
    VSAFree(p3);
    VSAFree(p4);

    printf("=== Before Allocating 60 ===\n");

    p5 = VSAAlloc(vsa, 60);
    assert(p5 != NULL);

    headers_after = CountHeaders(vsa);
    printf("Headers count after p5 allocation: %d\n", headers_after);

    printf("=== After Allocating 60 ===\n");

    printf("\nTestVSAAlloc passed\n");
}

static void PrintBlocks(vsa_t* vsa)
{
    header_t* curr = (header_t*)((char*)vsa + sizeof(vsa_t));
    printf("Blocks:\n");

    while ((char*)curr < (char*)vsa->end_pool)
    {
        printf("  [%s] %ld bytes\n", 
               (curr->block_size > 0) ? "Free" : "Used", 
               ABS(curr->block_size));

        curr = (header_t*)((char*)curr + sizeof(header_t) + ABS(curr->block_size));
    }

    printf("  [END]\n");
}



void TestVSAWriteAfterRealloc()
{
    const char* data;
    void* p4 = NULL;
    char* p2 = NULL;
    char* p3 = NULL;
    char* p1 = NULL;
    char pool[POOL_SIZE] = {0};
    vsa_t* vsa = VSAInit(pool, POOL_SIZE);

    p1 = VSAAlloc(vsa, 50);
    assert(p1 != NULL);
     p2 = VSAAlloc(vsa, 20);
    assert(p2 != NULL);
    p3 = VSAAlloc(vsa, 30);
    assert(p3 != NULL);

    VSAFree(p1);
    VSAFree(p2);
    VSAFree(p3);

    printf("Before allocating p4:\n");
	PrintBlocks(vsa);

    p4 = VSAAlloc(vsa, 60);
    assert(p4 != NULL);

    printf("After allocating p4:\n");
	PrintBlocks(vsa);

    data = "abcdefghijklmnopqrst";
    memcpy(p4, data, 20);
    memcpy(p2, data, 5);

    assert(memcmp(p4, data, 20) == 0);
    assert(memcmp(p2, data, 5) == 0);

    printf("TestVSAWriteAfterRealloc passed\n");
}

void TestVSAFree()
{
    void* p3 = NULL;
    char pool[POOL_SIZE] = {0};
    vsa_t* vsa = VSAInit(pool, POOL_SIZE);

    void* p1 = VSAAlloc(vsa, 16);
    void* p2 = VSAAlloc(vsa, 8);

    VSAFree(p1);
    VSAFree(p2);

    p3 = VSAAlloc(vsa, 24);  
    assert(p3 != NULL);

    printf("TestVSAFree passed\n");
}

void TestVSALargestChunkAvailable()
{
    size_t after = 0;
    size_t mid = 0;
    size_t before = 0;
    void* p1 = NULL;
    void* p2 = NULL;
    char pool[POOL_SIZE] = {0};
    vsa_t* vsa = VSAInit(pool, POOL_SIZE);

    before = VSALargestChunkAvailable(vsa);
	printf("%lu \n" ,before);
    assert(before > 0);

    p1 = VSAAlloc(vsa, 16);
    p2 = VSAAlloc(vsa, 9);

    mid = VSALargestChunkAvailable(vsa);
	printf("%lu \n" ,mid);
    assert(mid < before);

    VSAFree(p1);
    VSAFree(p2);

    after = VSALargestChunkAvailable(vsa);
    assert(after >= mid);
	printf("%lu \n" ,after);

    printf("TestVSALargestChunkAvailable passed\n");
}

int main()
{
    TestVSAInit();
    TestVSAAlloc();
    TestVSAFree();
    TestVSALargestChunkAvailable();
    TestVSAWriteAfterRealloc();

    printf("\nAll tests passed successfully!\n");
    return 0;
}
