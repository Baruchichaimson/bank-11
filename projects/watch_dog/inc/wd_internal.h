#ifndef __WD_INTERNAL_H__
#define __WD_INTERNAL_H__

#include <stddef.h>
#include <stdatomic.h>
#include <semaphore.h>
#include "sched.h"

extern const char *SEM_NAME_START;
extern const char *SEM_NAME_STOP;

typedef struct wd_context
{
    unsigned int interval;
    unsigned int tolerance;
    atomic_uint counter;       /* missed heartbeats */
    atomic_int stop_flag;     
    atomic_int wd_pid;             
    atomic_int user_pid;            
    int is_watchdog;          
    char **user_argv;    
    int user_argc;
    sched_t *user_sched;
    sem_t *start_sem;      
    sem_t *sem_stop;  

} wd_context_t;

void WDInternalSetContext(wd_context_t *ctx);
void WDSignalHandler(int sig);

ssize_t SendSigUSR1(void *param);   
ssize_t SendSigUSR2(void *param);   
ssize_t CheckStopFlag(void *param); 
ssize_t CheckTolerance(void *param);

void Revive(wd_context_t *ctx);   
int IsProcessAlive(pid_t pid);     

#endif /* __WD_INTERNAL_H__ */
