/***************************
 * Exercise: Scheduler
 * Date: 23/07/25
 * Developer: Baruch Haimson
 * Reviewer: Yael
 * Status: Approved
 ***************************/
 
#ifndef ILRD_SCHED_H
#define ILRD_SCHED_H

typedef struct sched sched_t;

#include "uid.h" /* uid API */

/**
 * Description: Creates a new scheduler object.
 *
 * Function Arguments: None.
 *
 * Return Value:
 *        - Pointer to a new scheduler object.
 *        - NULL if allocation fails.
 *
 * General notes: 
 *        - User must call SchedDestroy to free resources.
 *
 * Complexity:
 *        - Time: O(1).
 **/
sched_t* SchedCreate(void);

/**
 * Description: Destroys the scheduler and frees all associated resources.
 *
 * Function Arguments:
 *        sch - pointer to scheduler object.
 *
 * Return Value: None.
 *
 * General notes:
 *        - Behavior is undefined if sch is NULL.
 *
 * Complexity:
 *        - Time: O(n).
 **/
void SchedDestroy(sched_t* sch);

/**
 * Description: Runs the scheduler loop. Executes all scheduled tasks.
 *
 * Function Arguments:
 *        sch - pointer to scheduler object.
 *
 * Return Value:
 *        - 0 on success.
 *        - non-zero if a task returns non-zero.
 *
 * General notes:
 *        - Tasks are executed at their scheduled time.
 *        - Function blocks until stopped or all tasks are done.
 *
 * Complexity:
 *        - Time: O(n log n) (due to priority queue handling).
 **/
int SchedRun(sched_t* sch);

/**
 * Description: Stops the scheduler loop (used inside a task to stop execution).
 *
 * Function Arguments:
 *        sch - pointer to scheduler object.
 *
 * Return Value: None.
 *
 * General notes:
 *        - Stops the current run of the scheduler.
 *        - Safe to call from within a task.
 *
 * Complexity:
 *        - Time: O(1).
 **/
void SchedStop(sched_t* sch);

/**
 * Description: Adds a new task to the scheduler.
 *
 * Function Arguments:
 *        sch           - pointer to scheduler object.
 *        op_func       - pointer to function to execute.
 *        param         - parameter to pass to op_func.
 *        time_exe      - time in seconds from now to execute.
 *        cleanup_func  - pointer to cleanup function (can be NULL).
 *        cleanup_param - parameter to pass to cleanup_func.
 *
 * Return Value:
 *        - UID of the new task (ilrd_uid_t).
 *        - UIDBad if failed.
 *
 * General notes:
 *        - cleanup_func is called after the task completes.
 *        - op_func must return 0 to continue or non-zero to stop scheduler.
 *
 * Complexity:
 *        - Time: O(log n).
 **/
ilrd_uid_t SchedAdd(sched_t* sch, ssize_t (*op_func)(void* param), void* param,
                    size_t time_exe, void (*cleanup_func)(void* cleanup_param),
                    void* cleanup_param);

/**
 * Description: Removes a scheduled task by its UID.
 *
 * Function Arguments:
 *        sch - pointer to scheduler object.
 *        uid - UID of the task to remove.
 *
 * Return Value: None.
 *
 * General notes:
 *        - If uid not found, nothing happens.
 *
 * Complexity:
 *        - Time: O(n).
 **/
void SchedRemove(sched_t* sch, ilrd_uid_t uid);

/**
 * Description: Checks if the scheduler has no tasks.
 *
 * Function Arguments:
 *        sch - pointer to scheduler object.
 *
 * Return Value:
 *        - 1 if empty.
 *        - 0 if not empty.
 *
 * General notes: None.
 *
 * Complexity:
 *        - Time: O(1).
 **/
int SchedIsEmpty(const sched_t* sch);

/**
 * Description: Removes all tasks from the scheduler.
 *
 * Function Arguments:
 *        sch - pointer to scheduler object.
 *
 * Return Value: None.
 *
 * General notes:
 *        - All cleanup functions will be called.
 *
 * Complexity:
 *        - Time: O(n).
 **/
void SchedClear(sched_t* sch);

/**
 * Description: Returns the number of tasks currently in the scheduler.
 *
 * Function Arguments:
 *        sch - pointer to scheduler object.
 *
 * Return Value:
 *        - Number of scheduled tasks.
 *
 * General notes: None.
 *
 * Complexity:
 *        - Time: O(1).
 **/
size_t SchedSize(const sched_t* sch);

#endif /* ILRD_SCHED_H */

