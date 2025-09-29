#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include "wd_internal.h"

extern pthread_mutex_t g_revive_mutex;
extern sem_t *g_sem_start;
extern sem_t *g_sem_stop;
extern const char *g_sem_start_name;
extern const char *g_sem_stop_name;

static wd_context_t *g_ctx_ptr = NULL;

void WDInternalSetContext(wd_context_t *ctx)
{
    g_ctx_ptr = ctx;
}

void WDSignalHandler(int sig)
{
    if (!g_ctx_ptr) return;

    if (sig == SIGUSR1)
    {
        atomic_store(&g_ctx_ptr->counter, 0);
        printf("[signal][pid %d] SIGUSR1 received -> counter reset\n", getpid());
    }
    else if (sig == SIGUSR2)
    {
        atomic_store(&g_ctx_ptr->stop_flag, 1);
        printf("[signal][pid %d] SIGUSR2 received -> stop_flag set\n", getpid());
    }
}

int IsProcessAlive(pid_t pid)
{
    if (pid <= 0) 
    {
        return 0;
    }
    return kill(pid, 0) == 0 ? 1 : 0;
}

ssize_t SendSigUSR1(void *param)
{
    pid_t target_pid = 0;
    wd_context_t *ctx = (wd_context_t *)param;
    if (!ctx || atomic_load(&ctx->stop_flag)) 
    {
        return 0;
    }

    target_pid = ctx->is_watchdog ? ctx->user_pid : ctx->wd_pid;

    kill(target_pid, SIGUSR1);
       
    atomic_fetch_add(&ctx->counter, 1);

    printf("[SendSigUSR1][pid %d] Ping sent to pid %d -> counter=%u\n", getpid(), target_pid, atomic_load(&ctx->counter));

    return ctx->interval;
}

ssize_t SendSigUSR2(void *param)
{
    pid_t target_pid = 0;
    wd_context_t *ctx = (wd_context_t *)param;
    if (!ctx) 
    {
        return 0;
    }

    target_pid = ctx->wd_pid;

    if (IsProcessAlive(target_pid))
    {
        if (kill(target_pid, SIGUSR2) == 0)
        {
            printf("[SendSigUSR2][pid %d] SIGUSR2 sent to WD pid %d\n", getpid(), target_pid);
        }
        else
        {
            perror("[SendSigUSR2] kill(SIGUSR2) failed");
        }
    }

    if (ctx->user_sched)
    {
        printf("[SendSigUSR2][pid %d] Stopping user scheduler\n", getpid());
        SchedStop(ctx->user_sched);
    }

    return 0;
}

ssize_t CheckTolerance(void *param)
{
    unsigned int counter = 0;
    wd_context_t *ctx = (wd_context_t *)param;
    if (!ctx || atomic_load(&ctx->stop_flag)) 
    {
        return 0;
    }

    counter = atomic_load(&ctx->counter);

    if (counter > ctx->tolerance)
    {
        printf("[CheckTolerance][pid %d] Counter %u exceeded tolerance %u -> Revive()\n", getpid(), counter, ctx->tolerance);

        atomic_store(&ctx->counter, 0);
        Revive(ctx);
    }

    return ctx->interval;
}

ssize_t CheckStopFlag(void *param)
{
    wd_context_t *ctx = (wd_context_t *)param;
    if (!ctx)
    {
         return 0;
    }

    if (atomic_load(&ctx->stop_flag))
    {
        printf("[CheckStopFlag][pid %d] Stop flag detected -> stopping scheduler\n", getpid());

        if(ctx->user_sched)
        {
            SchedStop(ctx->user_sched); 
        }
        return -1;  
    }
    return ctx->interval;
}

void Revive(wd_context_t *ctx)
{
    int i = 0;
    pid_t pid = 0;
    pid_t old_wd_pid = 0;
    char interval_str[16];
    char tol_str[16];
    char* wd_args[4];

    if (!ctx)
    {
        return;
    }

    pthread_mutex_lock(&g_revive_mutex);

    if (atomic_load(&ctx->stop_flag) || getenv("WD_DO_NOT_REVIVE"))
    {
        printf("[Revive][pid %d] Stop requested or revive disabled -> skipping\n", getpid());
        pthread_mutex_unlock(&g_revive_mutex);
        return;
    }

    atomic_store(&ctx->counter, 0);
    atomic_store(&ctx->stop_flag, 0);

    if (!ctx->is_watchdog)
    {
        old_wd_pid = atomic_load(&ctx->wd_pid);

        printf("[Revive][pid %d] User reviving WD (old pid %d)\n", getpid(), old_wd_pid);

        if (IsProcessAlive(old_wd_pid))
        {
            kill(old_wd_pid, SIGTERM);
            for (i = 0; i < 5; ++i)
            {
                if (!IsProcessAlive(old_wd_pid)) 
                {
                    break;
                }
                usleep(200000);
            }
            if (IsProcessAlive(old_wd_pid))
            {
                kill(old_wd_pid, SIGKILL);
                waitpid(old_wd_pid, NULL, 0);
            }
        }

        pid = fork();
        if (pid < 0)
        {
            perror("[Revive] fork failed");
            pthread_mutex_unlock(&g_revive_mutex);
            return;
        }

        if (pid == 0)
        {
            snprintf(interval_str, sizeof(interval_str), "%u", ctx->interval);
            snprintf(tol_str, sizeof(tol_str), "%u", ctx->tolerance);

            wd_args[0] = "../bin/debug/watchdog";
            wd_args[1] = interval_str;
            wd_args[2] = tol_str;
            wd_args[3] = NULL;

            execvp(wd_args[0], wd_args);
            perror("[Revive][child] execvp failed");
            _exit(1);
        }

        atomic_store(&ctx->wd_pid, pid);
        printf("[Revive][pid %d] spawned new WD pid %d\n", getpid(), pid);
    }
    else
    {
        if (getenv("WD_DO_NOT_REVIVE"))
        {
            printf("[Revive][pid %d] Global stop detected, not reviving\n", getpid());
            pthread_mutex_unlock(&g_revive_mutex);
            return;
        }
        printf("[Revive][pid %d] WD reviving User via execvp...\n", getpid());

        wd_args[0] = "../bin/debug/user_prog", 
        wd_args[1] = NULL;

        execvp(wd_args[0], wd_args);
        perror("[Revive][child] execvp failed");
        _exit(1);
    }
}
