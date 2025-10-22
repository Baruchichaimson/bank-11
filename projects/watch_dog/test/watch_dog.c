#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#include "wd_internal.h"
#include "sched.h"

static void DummyCleanup(void *param) { (void)param; }

int main(int argc, char *argv[])
{
    wd_context_t ctx;
    struct sigaction sa;
    sem_t *wd_sem = NULL;
    sem_t *user_sem = NULL;
    sem_t *stop_sem = NULL;
    sched_t *sched = NULL;

    unsigned int interval = argc > 1 ? (unsigned int)atoi(argv[1]) : 2;
    unsigned int tolerance = argc > 2 ? (unsigned int)atoi(argv[2]) : 3;

    memset(&ctx, 0, sizeof(ctx));

    ctx.interval = interval;
    ctx.tolerance = tolerance;
    ctx.user_sched = NULL;
    ctx.is_watchdog = 1;          
    ctx.wd_pid = getpid();         
    ctx.user_pid = getppid();  

    WDInternalSetContext(&ctx);

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = WDSignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    sched = SchedCreate();
    if (!sched) 
    { 
        printf("Failed to create scheduler\n"); 
        return 1; 
    }

    SchedAdd(sched, SendSigUSR1, &ctx, interval, DummyCleanup, NULL);
    SchedAdd(sched, CheckTolerance, &ctx, interval, DummyCleanup, NULL);
    SchedAdd(sched, CheckStopFlag, &ctx, interval, DummyCleanup, NULL);

    wd_sem = sem_open(WD_SEM_START, 0);
    if (wd_sem != SEM_FAILED) 
    { 
        sem_post(wd_sem); 
        sem_close(wd_sem);
    }

    user_sem = sem_open(USER_SEM_START, 0);
    if (user_sem != SEM_FAILED) 
    { 
        sem_wait(user_sem); 
        sem_close(user_sem);
    }
    

    stop_sem = sem_open(WD_SEM_STOP, 0);
    if (stop_sem != SEM_FAILED)
    {
        ctx.sem_stop = stop_sem;
    }
    else
    {
        ctx.sem_stop = NULL; 
    }

    printf("[watch_dog][pid %d] WD scheduler starting\n", getpid());
    SchedRun(sched);
    SchedDestroy(sched);

    if (ctx.sem_stop)
    {
        sem_close(ctx.sem_stop);
    }

    return 0;
}

