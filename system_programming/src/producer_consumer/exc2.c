#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

#include "sll.h"

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_MESSAGES  10
#define QUEUE_MAX     20 

sll_t *queue;
int consumed_count = 0;
int finished_producing = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int messages[NUM_PRODUCERS][NUM_MESSAGES];

/* ----------------- Producer ----------------- */
/*{
    int id = (int)(size_t)arg;
    for (int i = 0; i < NUM_MESSAGES; i++)
    {
        messages[id][i] = id * 100 + i;

        while (1) 
        {
            pthread_mutex_lock(&lock);
            if (SLLCount(queue) < QUEUE_MAX) 
            {  
                SLLInsert(SLLEnd(queue), &messages[id][i]);
                printf("Producer %d produced %d\n", id, messages[id][i]);
                pthread_mutex_unlock(&lock);
                break;
            }
            pthread_mutex_unlock(&lock);
            sched_yield();
        }
    }
    return NULL;
}
*/
void *producer(void *arg)
{
    int inserted = 0;
    int id = (int)(size_t)arg;

    for (int i = 0; i < NUM_MESSAGES; i++)
    {
        messages[id][i] = id * 100 + i;

        inserted = 0;
        while (!inserted) 
        {
            pthread_mutex_lock(&lock);
            if (SLLCount(queue) < QUEUE_MAX) 
            {  
                SLLInsert(SLLEnd(queue), &messages[id][i]);
                printf("Producer %d produced %d\n", id, messages[id][i]);
                inserted = 1;
            }
            pthread_mutex_unlock(&lock);
            /* busy-waits */
        }
    }

    return NULL;
}

/* ----------------- Consumer ----------------- */
/*void *consumer(void *arg)
{
    int id = (int)(size_t)arg;

    while (1)
    {
        pthread_mutex_lock(&lock);
        if (!SLLIsEmpty(queue))
        {
            sll_iter_t it = SLLBegin(queue);
            int *msg = (int*)SLLGetData(it);
            SLLRemove(it);
            pthread_mutex_unlock(&lock);

            printf("Consumer %d consumed %d\n", id, *msg);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            sched_yield();  
        }
    }

    return NULL;
}*/

void *consumer(void *arg)
{
    int id = (int)(size_t)arg;
    int *msg = NULL;
    sll_iter_t it = {0};

    while (1)
    {
        pthread_mutex_lock(&lock);

        if (!finished_producing || !SLLIsEmpty(queue))
        {
            it = SLLBegin(queue);
            msg = (int*)SLLGetData(it);
            SLLRemove(it);
            consumed_count++;
            pthread_mutex_unlock(&lock);

            printf("Consumer %d consumed %d\n", id, *msg);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            /* busy-wait */
        }
    }

    return NULL;
}



/* ----------------- Main ----------------- */
int main()
{
    int done = 0;
    pthread_t prod[NUM_PRODUCERS];
    pthread_t cons[NUM_CONSUMERS];

    queue = SLLCreate();
    if (!queue) return 1;

    for (size_t i = 0; i < NUM_PRODUCERS; i++)
        pthread_create(&prod[i], NULL, producer, (void*)(size_t)i);

    for (size_t i = 0; i < NUM_CONSUMERS; i++)
        pthread_create(&cons[i], NULL, consumer, (void*)(size_t)i);

    for (size_t i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    finished_producing = 1;

    while (1)
    {
        pthread_mutex_lock(&lock);
        done = (consumed_count == NUM_PRODUCERS * NUM_MESSAGES);
        pthread_mutex_unlock(&lock);
        if (done) 
        {
            break;
        }
    }

    printf("All messages produced and consumed.\n");
    return 0;
}
