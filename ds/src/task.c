/***************************
 * Exercise: Task
 * Date: 22/07/25
 * Developer: Baruch Haimson
 * Reviwer: 
 * Status: In Progress
 ***************************/
 
#include <stdlib.h>
#include <assert.h>

#include "task.h"
#include "uid.h"

struct task
{
    ilrd_uid_t uid; 
	time_t time_to_execute;                        
    
	ssize_t (*op_func)(void* op_param);   
	void *op_param;                                 
	void (*cleanup_func)(void* cleanup_param);
	void* cleanup_param;   
};

task_t* TaskCreate(ssize_t (*op_func)(void *param),  void *op_param,size_t interval_in_sec,void (*cleanup_func)(void* param),void* cleanup_param)
{
	task_t* task = (task_t*)malloc(sizeof(task_t));
	if(!task)
	{
		return NULL;
	}
	ilrd_uid_t uid = UIDCreate();
	if(UIDIsSame(uid, UIDbadUID))
	{
		free(task);
		return NULL;
	}
	task->uid = uid;
	task->op_func =  op_func;
	task->op_param = op_param;  
	task->cleanup_func = cleanup_func;
	task->cleanup_param = cleanup_param;
	task->time_to_execute = time(NULL) + interval_in_sec;
	
	return task;
}

void TaskDestroy(task_t* task)
{
	assert(task);
		
	TaskCleanUp(task);

	free(task);
	
	task = NULL;
}

ilrd_uid_t TaskUID(const task_t* task)
{
	assert(task);
	
	return task->uid;
}

ssize_t TaskRun(task_t* task)
{
	assert(task);
		
	if (!task->op_func)
    {
        return -1; 
    }
    
	return task->op_func(task->op_param);
}

void TaskCleanUp(task_t* task)
{
	assert(task);
	
	if (task->cleanup_func)
	{
        task->cleanup_func(task->cleanup_param);
    }
}

void TaskSetTimeToRun(task_t* task, size_t interval_in_sec)
{
	assert(task);
	
	task->time_to_execute = time(NULL) + interval_in_sec;
}

size_t TaskGetTimeToRun(const task_t* task)
{
	assert(task);
	
	return task->time_to_execute;
}

int TaskIsMatch(const task_t* task, ilrd_uid_t uid)
{
	assert(task);
	
	return UIDIsSame(task->uid, uid);
}

int TaskCmp(const task_t* task1, const task_t* task2)
{
	assert(task1);
	assert(task2);

	return task1->time_to_execute == task2->time_to_execute;
}


