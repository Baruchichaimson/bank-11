#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc, free */
#include <unistd.h>     /* sleep */
#include <assert.h>    /* assert */

#include "sched.h" /* sched API */ 
#include "task.h" /* task API */ 
#include "uid.h" /* uid API */ 



static int run_counter = 0;

/* ------------ Dummy function that increments counter ------------ */
ssize_t CountingFunc(void *param)
{
    int id = *(int *)param;
    printf("CountingFunc #%d executed (run_counter = %d)\n", id, ++run_counter);

    return 0; // no re-run
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

/* ------------ Dummycleanup: free ------------ */
void func_free(void* id)
{
	free((int*)id);
}

/* ------------ DummyCleanup: Print ------------ */
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

/* ------------ DummyFunc: Func Fail Always  ------------ */
ssize_t FuncFailAlways(void *param)
{
    (void)param;
    printf("FuncFailAlways: failing\n");
    return -1; 
}

/* ------------ DummyFunc: Func Fail Once  ------------ */
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

/* ------------ DummyFunc: Stop run ------------ */
ssize_t StopFunc(void *param)
{
    sched_t *sch = (sched_t *)param;
    printf("Stopping scheduler from within task...\n");
    SchedStop(sch);
    return 0;
}

/* ------------ DummyFunc: Stop run and fail ------------ */
ssize_t StopAndFailFunc(void *param)
{
    sched_t *sched = (sched_t *)param;
    printf("StopAndFailFunc: stopping scheduler\n");
    SchedStop(sched);
    return -1; // כשל גם כן
}

/* ------------ DummyFunc: Counter  ------------ */
ssize_t DummyFuncConter(void *param)
{
    int *counter = (int *)param;
    ++(*counter);
    printf(" Counter: %d\n", *counter);
    return 0; // Don't reschedule
}

/* ------------ Tests ------------ */
void TestSchedCreateDestroy()
{
    printf("\n--- TestSchedCreateDestroy ---\n");

    sched_t *sched = SchedCreate();
    assert(sched);
    printf("SchedCreate: PASS\n");

    SchedDestroy(sched);
    printf("SchedDestroy: PASS\n");
}

void TestSchedAddAndIsEmpty()
{
    printf("\n--- TestSchedAddAndIsEmpty ---\n");

    sched_t *sched = SchedCreate();
    assert(sched);

    assert(SchedIsEmpty(sched) == 1);
    printf("SchedIsEmpty before add: PASS\n");

    dummy_data_t *data = CreateDummyData(3, 7);
    ilrd_uid_t uid = SchedAdd(sched, DummyFunc, data, 1, DummyCleanup, data);
    assert(!UIDIsSame(uid, UIDbadUID));
    printf("SchedAdd: PASS\n");

    assert(SchedIsEmpty(sched) == 0);
    printf("SchedIsEmpty after add: PASS\n");

    SchedDestroy(sched);
}

void TestSchedRun()
{
    printf("\n--- TestSchedRun ---\n");

    sched_t *sched = SchedCreate();
    assert(sched);

    run_counter = 0;

    for (int i = 0; i < 3; ++i)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;

        SchedAdd(sched, CountingFunc, id, i + 1, func_free, id);
    }

    printf("Running scheduler...\n");
    int status = SchedRun(sched);
    printf("SchedRun returned: %d\n", status);

    assert(run_counter == 3);
    printf("All 3 tasks executed: PASS\n");

    SchedDestroy(sched);
}

void TestSchedRemove()
{
    printf("\n--- TestSchedRemove ---\n");
    sched_t *sched = SchedCreate();
    assert(sched);

    dummy_data_t *data = CreateDummyData(5, 10);
    ilrd_uid_t uid = SchedAdd(sched, DummyFunc, data, 1, DummyCleanup, data);
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
    printf("\n--- TestSchedClearAndSize ---\n");

    sched_t *sched = SchedCreate();
    assert(sched);

    dummy_data_t *data1 = CreateDummyData(1, 2);
	dummy_data_t *data2 = CreateDummyData(3, 4);
	dummy_data_t *data3 = CreateDummyData(5, 6);

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
	printf("\n--- TestSchedRunWithStop ---\n");
	
    sched_t *sched = SchedCreate();
    int dummy_counter = 0;

    assert(sched);

    SchedAdd(sched, DummyFuncConter, &dummy_counter, 1, CleanupFuncNULL, NULL);

    SchedAdd(sched, StopFunc, sched, 2, CleanupFuncNULL, NULL);

    SchedAdd(sched, DummyFuncConter, &dummy_counter, 3, CleanupFuncNULL, NULL);

    printf("=== Running Scheduler (should stop early) ===\n");
    int status = SchedRun(sched);
    printf("SchedRun returned: %d \n", status);

    printf("Final counter value: %d (should be 1 if stopped early)\n", dummy_counter);

    SchedDestroy(sched);
}

void TestSchedFailedTasksNoEmptySched()
{
    printf("\n--- TestSchedFailedTasksNoEmptySched ---\n");
    
    sched_t *sched = SchedCreate();
    assert(sched);

    SchedAdd(sched, DummyFuncConter, &(int){0}, 1, CleanupFuncNULL, NULL);  
    SchedAdd(sched, StopAndFailFunc, sched, 2, CleanupFuncNULL, NULL);       
    SchedAdd(sched, FuncFailAlways, NULL, 3, CleanupFuncNULL, NULL);      

    int status = SchedRun(sched);
    printf("SchedRun returned: %d \n", status);


    SchedDestroy(sched);
}

void TestSchedFailedTasksEmptySched()
{
    printf("\n--- TestSchedTestSchedFailedTasksEmptySched ---\n");
    
    sched_t *sched = SchedCreate();
    assert(sched);

    int fail_flag = 0;

    dummy_data_t *data1 = CreateDummyData(1, 2);
    dummy_data_t *data2 = CreateDummyData(3, 4);

    SchedAdd(sched, DummyFunc, data1, 1, DummyCleanup, data1);      
    SchedAdd(sched, FuncFailOnce, &fail_flag, 1, CleanupFuncNULL, NULL); 
    SchedAdd(sched, DummyFunc, data2, 1, DummyCleanup, data2);     

    int status = SchedRun(sched);
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

