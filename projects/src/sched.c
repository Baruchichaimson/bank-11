/***************************
 * Exercise: Scheduler
 * Date: 23/07/25
 * Developer: Baruch Haimson
  * Reviewer: 
 * Status: In Progress
 ***************************/

#include <stdlib.h>

#include "task.h"
#include "pq.h"

struct sched
{
	pq_t* pq;
	int stop_flag;                        
    
};

int TaskCmp(const void* data1, const void* data2)
{
	return TaskCmp((const task_t*)data1, (const task_t*)data2);
}

static int TaskIsMatch(const void* data, const void* param)
{
	return TaskIsMatch((task_t*)data, *(ilrd_uid_t*)uid);
}


sched_t* SchedCreate(void)
{
	sched_t* sch = (sched_t*)malloc(sizeof(sched_t));
	if(!sch)
	{
		return NULL;
	}
	
	pq_t* pq = PQCreate(TaskCmp);
	if(!pq)
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
	
	PQDestroy(sch->pq);
	
	free(sch);
	
	sch = NULL;
}

int SchedRun(sched_t* sch)
{
	task_t* task = {0};
	assert(sch);
	
	while(!PQIsEmpty(sch->pq) && sch->stop_flag == 0)
	{
		task = PQPeek(sch->pq);
		TaskRun(task);
		PQRemove(sch->pq);
		if(0 < TaskRun(task))
		{
			
		}
		else if
	}
}

void SchedStop(sched_t* sch)
{
	assert(sch);

	sch->stop_flag = 1;
}

ilrd_uid_t SchedAdd(sched_t* sch, int(op_func*)(void* param), void* param, size_t time_exe, void(cleanup_func*), void* cleanup_param)
{
	task_t* task = {0};

	assert(sch);

	task = TaskCreate(op_func, param, time_exe, cleanup_func, cleanup_param);
	if(NULL == task)
	{
		return NULL;
	}
	
	if(PQEnqueue(sch->pq , task))
	{
		return NULL;
	}
	
	return TaskUID(task);
}

void SchedRemove(sched_t* sch, ilrd_uid_t uid)
{
	assert(sch);

	PQErase(sch->pq, TaskIsMatch, uid);
}

int SchedIsEmpty(const sched_t* sch)
{
	assert(sch);
	
	return PQIsEmpty(sch->pq);
}

void SchedClear(sched_t* sch)
{
	task_t* task = PQPeek(sch->pq);
	
	assert(sch);
	
	while(!PQIsEmpty(pq))
	{
		TaskDestroy(task);
	}
	
	PQClear(sch->pq)
}

size_t SchedSize(const sched_t* sch)
{
	assert(sch);
}
