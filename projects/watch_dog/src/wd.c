#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> /* O_CREAT */
#include <sys/stat.h> /* 0666 */
#include <sys/types.h>
#include <string.h>
#include "wd.h"
#include "wd_internal.h"

pthread_mutex_t g_revive_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t *g_sem_start = NULL;
sem_t *g_sem_stop  = NULL;

const char *g_sem_start_name = "/wd_start_sem";
const char *g_sem_stop_name  = "/wd_stop_sem";

static wd_context_t g_ctx;
static pthread_t g_thread_id = 0;

static void NoCleanup(void *param) 
{ 
    (void)param; 
}

static void *UserSchedThread(void *arg)
{
    wd_context_t *ctx = (wd_context_t *)arg;
    if (!ctx) return NULL;

    sched_t *user_sched = ctx->user_sched = SchedCreate();
    if (!user_sched) return NULL;

    SchedAdd(user_sched, SendSigUSR1, ctx, ctx->interval, NoCleanup, NULL);
    SchedAdd(user_sched, CheckTolerance, ctx, ctx->interval, NoCleanup, NULL);
    SchedAdd(user_sched, CheckStopFlag, ctx, ctx->interval, NoCleanup, NULL);

    SchedRun(user_sched);

    printf("[UserSchedThread][pid %d] User scheduler exiting\n", getpid());
    return NULL;
}

wd_status_e WDStart(unsigned int interval, unsigned int tolerance, const char **argv, int argc)
{
    int i = 0;
    pid_t pid = 0;
    char interval_str[16];
    char tol_str[16];
    char *wd_args[4];
    struct sigaction sa = {0};
    

    g_ctx.interval = interval;
    g_ctx.tolerance = tolerance;
    atomic_store(&g_ctx.counter, 0);
    atomic_store(&g_ctx.stop_flag, 0);
    g_ctx.is_watchdog = 0;
    atomic_store(&g_ctx.user_pid, getpid());
    atomic_store(&g_ctx.wd_pid, -1);
    g_ctx.user_sched = NULL;
    g_ctx.sem_stop = NULL;

    WDInternalSetContext(&g_ctx);

    g_ctx.user_argc = argc;
    g_ctx.user_argv = malloc(sizeof(char*) * (argc + 1)); 
    if (!g_ctx.user_argv)
    {
        perror("malloc failed");
        return FAIL;
    }
    for (i = 0; i < argc; ++i)
    {
        g_ctx.user_argv[i] = strdup(argv[i]);
    }
    g_ctx.user_argv[argc] = NULL; 

    sem_unlink(g_sem_start_name);
    sem_unlink(g_sem_stop_name);

    g_sem_start = sem_open(g_sem_start_name, O_CREAT, 0666, 0);
    g_sem_stop  = sem_open(g_sem_stop_name,  O_CREAT, 0666, 0);

    if (g_sem_start == SEM_FAILED || g_sem_stop == SEM_FAILED)
    {
        perror("sem_open failed");
        return FAIL;
    }

    sa.sa_handler = WDSignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return FAIL;
    }

    if (pid == 0)
    {
        snprintf(interval_str, sizeof(interval_str), "%u", interval);
        snprintf(tol_str, sizeof(tol_str), "%u", tolerance);

        wd_args[0] = "../bin/debug/watchdog";
        wd_args[1] = interval_str;
        wd_args[2] = tol_str;
        wd_args[3] = NULL;

        execvp(wd_args[0], wd_args);
        perror("execvp failed");
        _exit(1);
    }

    atomic_store(&g_ctx.wd_pid, pid);
    printf("[WDStart][pid %d] WD started with pid %d\n", getpid(), pid);

    sem_wait(g_sem_start);

    if (pthread_create(&g_thread_id, NULL, UserSchedThread, &g_ctx) != 0)
    {
        perror("pthread_create failed");
        return FAIL;
    }

    return SUCCESS;
}

wd_status_e WDStop(void)
{
    printf("[WDStop][pid %d] Stopping WD and user scheduler\n", getpid());

    setenv("WD_DO_NOT_REVIVE", "1", 1);

    if (g_ctx.wd_pid > 0)
    {
        if (kill(g_ctx.wd_pid, SIGUSR2) != 0)
        {
            perror("kill(SIGUSR2) failed");
        }
    }

    atomic_store(&g_ctx.stop_flag, 1);

    /*sem_wait(g_sem_stop);*/

    if (g_sem_stop && g_sem_stop != SEM_FAILED)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 5; /* wait max 5 seconds */

        if (sem_timedwait(g_sem_stop, &ts) != 0)
        {
            perror("[WDStop] sem_timedwait failed or timed out");
        }
    }

    if (g_ctx.user_sched)
    {
        SchedDestroy(g_ctx.user_sched);
        g_ctx.user_sched = NULL;
    }

    if (g_thread_id)
    {
        pthread_join(g_thread_id, NULL);
        g_thread_id = 0;
    }

    if (g_sem_start && g_sem_start != SEM_FAILED)
    {
        sem_close(g_sem_start);
        sem_unlink(g_sem_start_name);
        g_sem_start = NULL;
    }
    if (g_sem_stop && g_sem_stop != SEM_FAILED)
    {
        sem_close(g_sem_stop);
        sem_unlink(g_sem_stop_name);
        g_sem_stop = NULL;
    }

    if (g_ctx.user_argv)
    {
        for (int i = 0; i < g_ctx.user_argc; ++i)
        {
            free(g_ctx.user_argv[i]);
        }
        free(g_ctx.user_argv);
        g_ctx.user_argv = NULL;
    }

    if (g_ctx.wd_pid > 0)
    {
        int status = 0;
        pid_t ret = waitpid(g_ctx.wd_pid, &status, WNOHANG);
        if (ret == 0)
        {
            /* still running, force kill */
            kill(g_ctx.wd_pid, SIGKILL);
            waitpid(g_ctx.wd_pid, &status, 0);
        }
        g_ctx.wd_pid = -1;
    }

    printf("[WDStop][pid %d] WD and scheduler stopped cleanly\n", getpid());
    return SUCCESS;
}
