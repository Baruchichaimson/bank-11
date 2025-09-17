#include <stdio.h>      /* printf */
#include <stdlib.h>     /* exit */
#include <pthread.h>    /* pthread_create, pthread_join */
#include "uid.h"        /* ilrd_uid_t, UIDCreate, UIDIsSame */

#define NUM_THREADS 5
#define NUM_UIDS_PER_THREAD 3

/* ------------ Thread function ------------ */
void *thread_func(void *arg)
{
    int thread_num = *(int *)arg;
    int i;
    for (i = 0; i < NUM_UIDS_PER_THREAD; ++i)
    {
        ilrd_uid_t uid = UIDCreate();
        printf("Thread %d -> UID: counter=%lu, timestamp=%ld, pid=%d, ip=%s\n",
               thread_num,
               (unsigned long)uid.counter,
               (long)uid.timestamp,
               uid.pid,
               uid.ip);
    }
    return NULL;
}

/* ------------ Main test ------------ */
int main(void)
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; ++i)
    {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_func, &ids[i]) != 0)
        {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nTest finished.\n");
    return 0;
}
