
/***************************
 * Exercise: Scheduler
 * Date: 27/08/25
 * Developer: Baruch Haimson
 * Reviewer: tamar
 * Status: Approved
 ***************************/

#include <stdlib.h> /* malloc */
#include <unistd.h> /* sleep */
#include <assert.h> /* assert */

#include "sched.h" /* sched API */
#include "task.h" /* Task API */
#include "uid.h" /* uid API */
#include "pq_heap.h" /* pq API */

struct sched
{
	pq_heap_t* pq_heap;
	int stop_flag;                        
    
};

enum status { SUCCESS, FAILED_TASKS_EMPTY_SCHED, PAUSED, FAILED_TASKS_NO_EMPTY_SCHED , FAIL_ENQUEUE, UNKNOWN_STATE};

/***************** helper function *******************************/

static int definedCases(sched_t *sch, int failed);
static int WrapperTaskCmp(const void *data1, const void *data2);
static int WrapperTaskIsMatch(const void *data, const void *param);

/****************************************************************/

sched_t* SchedCreate(void)
{
	sched_t* sch = (sched_t*)malloc(sizeof(sched_t));
	if(!sch)
	{
		return NULL;
	}
	
	sch->pq_heap = PQHeapCreate(WrapperTaskCmp);
	if(!sch->pq_heap)
	{
		free(sch);
		return NULL;
	}
	
	sch->stop_flag = 0;
	
	return sch;
}

void SchedDestroy(sched_t* sch)
{
	assert(sch);

	SchedClear(sch);
	
	PQHeapDestroy(sch->pq_heap);
	
	free(sch);
	
	sch = NULL;
}

int SchedRun(sched_t* sch)
{
	ssize_t time_val  = 0;
	task_t* task = {0};
	int failed = 0;
	
	sch->stop_flag = 0;
	
	assert(sch);
	
	while(!PQHeapIsEmpty(sch->pq_heap) && sch->stop_flag == 0)
	{
		task = PQHeapDequeue(sch->pq_heap);
		while(TaskGetTimeToRun(task) > (size_t)time(NULL))
		{
			sleep(TaskGetTimeToRun(task) - (size_t)time(NULL));
		}
		
		time_val = TaskRun(task);
		
		if(0 < time_val)
		{
			TaskSetTimeToRun(task, time_val);
			
			if(PQHeapEnqueue(sch->pq_heap , task))
			{
				TaskDestroy(task);
				return FAIL_ENQUEUE;
			}
			
		}
		else if(-1 == time_val)
		{
			TaskDestroy(task);
			failed = 1;
		}
		else
		{
    		TaskDestroy(task);
		}
	}

	return definedCases(sch,failed);
}

void SchedStop(sched_t* sch)
{
	assert(sch);

	sch->stop_flag = 1;
}

ilrd_uid_t SchedAdd(sched_t* sch, ssize_t (*op_func)(void* param), void* param, size_t time_exe, void (*cleanup_func)(void* cleanup_param), void* cleanup_param)
{
	task_t* task = NULL;

	assert(sch);
	assert(op_func);
	assert(cleanup_func);

	task = TaskCreate(op_func, param, time_exe, cleanup_func, cleanup_param);
	if(NULL == task)
	{
		return UIDbadUID;
	}
	
	if(PQHeapEnqueue(sch->pq_heap , task))
	{
		TaskDestroy(task);
		return UIDbadUID;
	}
	
	return TaskUID(task);
}

void SchedRemove(sched_t* sch, ilrd_uid_t uid)
{
    task_t *task = NULL;

    assert(sch);

	task = PQHeapErase(sch->pq_heap, WrapperTaskIsMatch, &uid);
	
	if (task != NULL)
    {
        TaskDestroy(task);  
    }
}

int SchedIsEmpty(const sched_t* sch)
{
	assert(sch);
	
	return PQHeapIsEmpty(sch->pq_heap);
}

void SchedClear(sched_t* sch)
{
	task_t* task = NULL;
	
	assert(sch);
	
	while (!PQHeapIsEmpty(sch->pq_heap))
	{
		 task = (task_t*)PQHeapDequeue(sch->pq_heap);
         TaskDestroy(task);  
	}

	
	PQHeapClear(sch->pq_heap);
}

size_t SchedSize(const sched_t* sch)
{
	assert(sch);

	return PQHeapSize(sch->pq_heap);
}

/**************** helper function **********************/
static int definedCases(sched_t *sch, int failed)
{
    if (SchedIsEmpty(sch))
    {
        return failed ? FAILED_TASKS_EMPTY_SCHED : SUCCESS;
    }

    if (sch->stop_flag)
    {
        return failed ? FAILED_TASKS_NO_EMPTY_SCHED : PAUSED;
    }

    return UNKNOWN_STATE; 
}


static int WrapperTaskCmp(const void *data1, const void *data2)
{
    return TaskCmp((const task_t *)data1, (const task_t *)data2);
}

static int WrapperTaskIsMatch(const void *data, const void *param)
{
    return TaskIsMatch((const task_t *)data, *(const ilrd_uid_t *)param);
}

