#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc, free */
#include <unistd.h>     /* sleep */
#include <assert.h>     /* assert */

#include "sched.h" /* sched API */ 
#include "task.h"  /* task API */ 
#include "uid.h"   /* uid API */ 

static int run_counter = 0;

/* ------------ Dummy function that increments counter ------------ */
ssize_t CountingFunc(void *param)
{
    int id = *(int *)param;
    printf("CountingFunc #%d executed (run_counter = %d)\n", id, ++run_counter);

    return 0; /* no re-run */
}

/* ------------ Struct for DummyFunc ------------ */
typedef struct
{
    int x;
    int y;
} dummy_data_t;

/* ------------ DummyFunc: Sum and Print ------------ */
ssize_t DummyFunc(void *param)
{
    dummy_data_t *data = (dummy_data_t *)param;
    int sum = data->x + data->y;

    printf("DummyFunc called: %d + %d = %d\n", data->x, data->y, sum);

    return 0;
}

/* ------------ Dummycleanup: cleanup ------------ */
void CleanupFuncNULL(void* param)
{
	(void)param;
}

void func_free(void* id)
{
	free((int*)id);
}

void DummyCleanup(void *param)
{
    free(param);  
    printf("Cleanup called\n");
}

/* ------------ Helper: Create dummy_data_t ------------ */
dummy_data_t *CreateDummyData(int x, int y)
{
    dummy_data_t *data = (dummy_data_t *)malloc(sizeof(dummy_data_t));
    assert(data);

    data->x = x;
    data->y = y;

    return data;
}

ssize_t FuncFailAlways(void *param)
{
    (void)param;
    printf("FuncFailAlways: failing\n");
    return -1; 
}

ssize_t FuncFailOnce(void *param)
{
    int *has_failed = (int *)param;
    
    if (*has_failed == 0)
    {
        *has_failed = 1;
        printf("FuncFailOnce: failing now\n");
        return -1;  
    }
    
    printf("FuncFailOnce: success\n");
    return 0;  
}

ssize_t StopFunc(void *param)
{
    sched_t *sch = (sched_t *)param;
    printf("Stopping scheduler from within task...\n");
    SchedStop(sch);
    return 0;
}

ssize_t StopAndFailFunc(void *param)
{
    sched_t *sched = (sched_t *)param;
    printf("StopAndFailFunc: stopping scheduler\n");
    SchedStop(sched);
    return -1; /* also fail */
}

ssize_t DummyFuncConter(void *param)
{
    int *counter = (int *)param;
    ++(*counter);
    printf(" Counter: %d\n", *counter);
    return 0;
}

/* ------------ Tests ------------ */
void TestSchedCreateDestroy()
{
    sched_t *sched;

    printf("\n--- TestSchedCreateDestroy ---\n");

    sched = SchedCreate();
    assert(sched);
    printf("SchedCreate: PASS\n");

    SchedDestroy(sched);
    printf("SchedDestroy: PASS\n");
}

void TestSchedAddAndIsEmpty()
{
    sched_t *sched;
    dummy_data_t *data;
    ilrd_uid_t uid;

    printf("\n--- TestSchedAddAndIsEmpty ---\n");

    sched = SchedCreate();
    assert(sched);

    assert(SchedIsEmpty(sched) == 1);
    printf("SchedIsEmpty before add: PASS\n");

    data = CreateDummyData(3, 7);
    uid = SchedAdd(sched, DummyFunc, data, 1, DummyCleanup, data);
    assert(!UIDIsSame(uid, UIDbadUID));
    printf("SchedAdd: PASS\n");

    assert(SchedIsEmpty(sched) == 0);
    printf("SchedIsEmpty after add: PASS\n");

    SchedDestroy(sched);
}

void TestSchedRun()
{
    sched_t *sched;
    int i;
    int status;

    printf("\n--- TestSchedRun ---\n");

    sched = SchedCreate();
    assert(sched);

    run_counter = 0;

    for (i = 0; i < 3; ++i)
    {
        int *id = (int *)malloc(sizeof(int));
        *id = i + 1;
        SchedAdd(sched, CountingFunc, id, i + 1, func_free, id);
    }

    printf("Running scheduler...\n");
    status = SchedRun(sched);
    printf("SchedRun returned: %d\n", status);

    assert(run_counter == 3);
    printf("All 3 tasks executed: PASS\n");

    SchedDestroy(sched);
}

void TestSchedRemove()
{
    sched_t *sched;
    dummy_data_t *data;
    ilrd_uid_t uid;

    printf("\n--- TestSchedRemove ---\n");
    sched = SchedCreate();
    assert(sched);

    data = CreateDummyData(5, 10);
    uid = SchedAdd(sched, DummyFunc, data, 1, DummyCleanup, data);
    assert(!UIDIsSame(uid, UIDbadUID));
    printf("SchedAdd: PASS\n");

    printf("Before Remove:\n");
	printf("SchedSize before Remove: %lu\n", SchedSize(sched));

    SchedRemove(sched, uid);
    printf("SchedRemove: PASS\n");

    printf("After Remove:\n");
	printf("SchedSize after Remove: %lu\n", SchedSize(sched));
    assert(SchedIsEmpty(sched));
    printf("SchedIsEmpty after remove: PASS\n");

    SchedDestroy(sched);
}

void TestSchedClearAndSize()
{
    sched_t *sched;
    dummy_data_t *data1;
    dummy_data_t *data2;
    dummy_data_t *data3;

    printf("\n--- TestSchedClearAndSize ---\n");

    sched = SchedCreate();
    assert(sched);

    data1 = CreateDummyData(1, 2);
	data2 = CreateDummyData(3, 4);
	data3 = CreateDummyData(5, 6);

	SchedAdd(sched, DummyFunc, data1, 1, DummyCleanup, data1);
	SchedAdd(sched, DummyFunc, data2, 2, DummyCleanup, data2);
	SchedAdd(sched, DummyFunc, data3, 3, DummyCleanup, data3);

    printf("SchedSize before clear: %lu\n", SchedSize(sched));

    SchedClear(sched);
    printf("SchedClear: PASS\n");

    assert(SchedIsEmpty(sched));
    printf("SchedIsEmpty after clear: PASS\n");

    SchedDestroy(sched);
}

void TestSchedRunWithStop()
{
    sched_t *sched;
    int dummy_counter;
    int status;

	printf("\n--- TestSchedRunWithStop ---\n");
	
    sched = SchedCreate();
    dummy_counter = 0;

    assert(sched);

    SchedAdd(sched, DummyFuncConter, &dummy_counter, 1, CleanupFuncNULL, NULL);
    SchedAdd(sched, StopFunc, sched, 2, CleanupFuncNULL, NULL);
    SchedAdd(sched, DummyFuncConter, &dummy_counter, 3, CleanupFuncNULL, NULL);

    printf("=== Running Scheduler (should stop early) ===\n");
    status = SchedRun(sched);
    printf("SchedRun returned: %d \n", status);

    printf("Final counter value: %d (should be 1 if stopped early)\n", dummy_counter);

    SchedDestroy(sched);
}

void TestSchedFailedTasksNoEmptySched()
{
    sched_t *sched;
    int init_counter;
    int status;

    printf("\n--- TestSchedFailedTasksNoEmptySched ---\n");
    
    sched = SchedCreate();
    assert(sched);

    init_counter = 0;

    SchedAdd(sched, DummyFuncConter, &init_counter, 1, CleanupFuncNULL, NULL);  
    SchedAdd(sched, StopAndFailFunc, sched, 2, CleanupFuncNULL, NULL);       
    SchedAdd(sched, FuncFailAlways, NULL, 3, CleanupFuncNULL, NULL);      

    status = SchedRun(sched);
    printf("SchedRun returned: %d \n", status);

    SchedDestroy(sched);
}

void TestSchedFailedTasksEmptySched()
{
    sched_t *sched;
    int fail_flag;
    dummy_data_t *data1;
    dummy_data_t *data2;
    int status;
    
    printf("\n--- TestSchedTestSchedFailedTasksEmptySched ---\n");
    
    sched = SchedCreate();
    assert(sched);

    fail_flag = 0;

    data1 = CreateDummyData(1, 2);
    data2 = CreateDummyData(3, 4);

    SchedAdd(sched, DummyFunc, data1, 1, DummyCleanup, data1);      
    SchedAdd(sched, FuncFailOnce, &fail_flag, 1, CleanupFuncNULL, NULL); 
    SchedAdd(sched, DummyFunc, data2, 1, DummyCleanup, data2);     

    status = SchedRun(sched);
    printf("SchedRun returned: %d \n", status);

    SchedDestroy(sched);
}


int main()
{
    TestSchedCreateDestroy();
    TestSchedAddAndIsEmpty();
    TestSchedRemove();
    TestSchedClearAndSize();
    TestSchedRun();
	TestSchedRunWithStop();
    TestSchedFailedTasksNoEmptySched();
	TestSchedFailedTasksEmptySched();
	
    printf("\nAll sched tests passed\n");
    return 0;
}
