#include "vsa.h"

#include <stdlib.h>
#include <stdio.h>

#define WORD			(8)
#define BLOCK_SIZE		(WORD)
#define VSA_SIZE 		(WORD * 3)
#define HEADER_SIZE		(WORD * 2)
#define POOL_SIZE(x)		(((size_t)(*(void**)x)) - (size_t)(x) - VSA_SIZE)
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

static int global_result = 1;
static int current_test = 1; 
static char* TestPrintStr = NULL;

static int PrintResults(int result, char* test_info);
static void PrintTestResults(void);

static void TestInit(void);
static void Test2(void);
static void Test3(void);

/*
void* VSAAlloc(vsa_t* vsa, size_t block_size);
void VSAFree(void* block);
size_t VSALargestChunkAvailable(vsa_t* vsa);
*/

int main()
{
	
	
	TestInit();
	Test2();
	Test3();


return 0;
}
/****************************************/
/*	STATIC TEST FUNCS		*/
/****************************************/
static void Test3(void)
{
	vsa_t* vsa = (vsa_t*)malloc(2000);
	void* p1 = NULL; 
	void* p2 = NULL; 
	void* p3 = NULL; 
	void* p4 = NULL; 
	size_t largest = 240 + 360 + 400  + HEADER_SIZE * 2;
	current_test = 1;

	printf(BOLD"#### Test 3 \n"RESET);
	TestPrintStr = " Test 3:";
	
	printf("Allocating 240 + 360 + 400 + VSA_SIZE + HEADER_SIZE * 2 = %lu\n", largest);
	vsa = VSAInit(vsa, largest + VSA_SIZE);
	
 	PrintResults(2 * HEADER_SIZE  + 1000  == VSALargestChunkAvailable(vsa), "2 * HEADER_SIZE  + 1000 == VSALargestChunkAvailable(vsa)");
	printf("allocating: p1 240, p2 320, p3 400, p4 8\n");
	p1 = VSAAlloc(vsa, 240);
	largest -= 240 + HEADER_SIZE;
	PrintResults(NULL != p1, "NULL != p1");
	printf("chunk = %lu -",VSALargestChunkAvailable(vsa));
	PrintResults(largest == VSALargestChunkAvailable(vsa), "VSALargestChunkAvailable(vsa) is 240+header smaller");

	p2 = VSAAlloc(vsa, 320);
	largest -= 320 + HEADER_SIZE;
	PrintResults(NULL != p2, "NULL != p2");
	PrintResults(largest == VSALargestChunkAvailable(vsa), "VSALargestChunkAvailable(vsa) is 320+header smaller");
	
	p3 = VSAAlloc(vsa, 400);
	largest -= 400 + HEADER_SIZE;
	PrintResults(NULL != p3, "NULL != p3");	
	printf("chunk = %lu -",VSALargestChunkAvailable(vsa));
	PrintResults(largest == VSALargestChunkAvailable(vsa), "VSALargestChunkAvailable(vsa)is 400+header smaller");
	
	p4 = VSAAlloc(vsa, 8);
	largest = 0;
	PrintResults(NULL != p4, "NULL != p4");
	printf("chunk = %lu -",VSALargestChunkAvailable(vsa));
	PrintResults(0 == VSALargestChunkAvailable(vsa), "VSALargestChunkAvailable(vsa) is 0");
	VSAFree(p4);
	PrintResults(24 == VSALargestChunkAvailable(vsa), "freed p4: VSALargestChunkAvailable(vsa) is 24");
	VSAFree(p2);
	PrintResults(320 == VSALargestChunkAvailable(vsa), "freed p2: VSALargestChunkAvailable(vsa) is 320");
	VSAFree(p3);
	PrintResults(320 + 400 + 24 + 2*HEADER_SIZE == VSALargestChunkAvailable(vsa), "freed p4: VSALargestChunkAvailable(vsa) is 320 + 400 + 24 + 2*HEADER_SIZE");

	p2 = VSAAlloc(vsa,8);
	p3 = VSAAlloc(vsa,16);
	PrintResults(320 + 400 == VSALargestChunkAvailable(vsa), "allocated 8,16: VSALargestChunkAvailable(vsa) is 320 + 400");
	VSAFree(p1);
	VSAFree(p3);
	VSAFree(p2);
 	PrintResults(2 * HEADER_SIZE  + 1000  == VSALargestChunkAvailable(vsa), "freed all, VSALargestChunkAvailable(vsa) as start");
	
	free(vsa);
	PrintTestResults();
}


static void Test2(void)
{
	vsa_t* vsa = (vsa_t*)malloc(1000);
	void* p = NULL; 
	current_test = 1;
	
	printf(BOLD"#### Test 2 \n"RESET);
	TestPrintStr = " Test 2:";
	
	vsa = VSAInit(vsa,80 + VSA_SIZE);
	printf("allocated 80 + VSA_SIZE\n");
	PrintResults(80 == POOL_SIZE(vsa), "80 == POOL_SIZE(vsa)");
	p = VSAAlloc(vsa, 80);
	PrintResults(NULL != p, "NULL != VSAAlloc(vsa, 80)");
	PrintResults(NULL == VSAAlloc(vsa, 8), "NULL == VSAAlloc(vsa, 8)");
	VSAFree(p);
	PrintResults(1, "Freed the block");
	PrintResults(80 == VSALargestChunkAvailable(vsa), "80 == VSALargestChunkAvailable(vsa)");
	printf("largest = %lu\n", VSALargestChunkAvailable(vsa));
	p = VSAAlloc(vsa, 64);
	PrintResults(NULL != p, "NULL != VSAAlloc(vsa, 72) - again");
	
	
	free(vsa);
	PrintTestResults();
}

static void TestInit(void)
{
	vsa_t* vsa = (vsa_t*)malloc(1000);
	char* p = (char*)vsa; 
	current_test = 1;
	
	printf(BOLD"#### VSAInit test\n"RESET);
	TestPrintStr = "VSAInit test:";
	
	printf(BOLD"vsa mod WORD = %lu\n"RESET, (size_t)vsa % WORD); 
	
	printf("VSAInitr(vsa,96)\n");
	vsa = VSAInit(vsa,96);
	PrintResults(p == (char*)vsa , "vsa is same address");
	PrintResults((96 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(96 - VSA_SIZE) ==  POOL_SIZE(vsa)");

	printf("VSAInitr(vsa,97)\n");
	vsa = VSAInit(vsa,97);
	PrintResults(p == (char*)vsa , "vsa is same address");
	PrintResults((96 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(96 - VSA_SIZE) ==  POOL_SIZE(vsa)");
	
	vsa = VSAInit(vsa,95);
	PrintResults(p == (char*)vsa , "vsa is same address");
	PrintResults((88 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(88 - VSA_SIZE) ==  POOL_SIZE(vsa)");

	
	++p;
	printf(BOLD"\nvsa mod WORD = %lu\n"RESET, (size_t)p % WORD); 
	printf("VSAInitr(vsa,96)\n");
	vsa = VSAInit(p,96);
	PrintResults((char*)(p+7) == (char*)vsa , "vsa has moved 7 bytes");
	PrintResults((88 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(88 - VSA_SIZE) ==  POOL_SIZE(vsa)");
	 
	printf("VSAInitr(vsa,97)\n");
	vsa = VSAInit(p,97);
	PrintResults((char*)(p+7) == (char*)vsa , "vsa has moved 7 bytes");
	PrintResults((88 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(88 - VSA_SIZE) ==  POOL_SIZE(vsa)");
	
	printf("VSAInitr(vsa,95)\n");
	vsa = VSAInit(p,95);
	PrintResults((char*)(p+7) == (char*)vsa , "vsa has moved 7 bytes");
	PrintResults((88 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(88 - VSA_SIZE) ==  POOL_SIZE(vsa)");
	
	p = p-2; 
	
	printf(BOLD"\nvsa mod WORD = %lu\n"RESET, (size_t)p % WORD); 
	printf("VSAInitr(vsa,96)\n");
	vsa = VSAInit(p,96);
	PrintResults((char*)(p+1) == (char*)vsa , "vsa has moved 1 bytes");
	PrintResults((88 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(88 - VSA_SIZE) ==  POOL_SIZE(vsa)");
	 
	printf("VSAInitr(vsa,97)\n");
	vsa = VSAInit(p,97);
	PrintResults((char*)(p+1) == (char*)vsa , "vsa has moved 1 bytes");
	PrintResults((96 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(96 - VSA_SIZE) ==  POOL_SIZE(vsa)");
	
	printf("VSAInitr(vsa,95)\n");
	vsa = VSAInit(p,95);
	PrintResults((char*)(p+1) == (char*)vsa , "vsa has moved 1 bytes");
	PrintResults((88 - VSA_SIZE) ==  POOL_SIZE(vsa) , "(88 - VSA_SIZE) ==  POOL_SIZE(vsa)");
	
	free(vsa);
	PrintTestResults();
}

/****************************************/
/*	STATIC HELPER FUNCS		*/
/****************************************/

static void PrintTestResults(void)
{
	if (current_test)
	{
		printf(GREEN"============ PASSED ALL TEST IN %s ============"RESET"\n\n", TestPrintStr);
	}
	else
	{
		printf(RED"============ FAILED SOME TEST IN %s ============"RESET"\n\n", TestPrintStr);	
	}
}

static int PrintResults(int result, char* test_info)
{
	if (result)
	{
		printf(GREEN"Passed %s"RESET"\n",test_info);
	}
	else
	{
		global_result = 0;
		current_test = 0;
		printf(RED"Failed %s"RESET"\n",test_info);		
	}
	
	return result;
}



