#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include "wd_internal.h"
#include "sched.h"

#define SEM_NAME "/wd_sync_sem"
static void DummyCleanup(void *param) { (void)param; }

int main(int argc, char *argv[])
{
    wd_context_t ctx;
    struct sigaction sa;
    sem_t *sem = NULL;
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

    sem = sem_open(SEM_NAME, 0);
    if (sem != SEM_FAILED) 
    { 
        sem_post(sem); 
        sem_close(sem); 
    }

    printf("[watch_dog][pid %d] WD scheduler starting\n", getpid());
    SchedRun(sched);
    SchedDestroy(sched);

    return 0;
}

