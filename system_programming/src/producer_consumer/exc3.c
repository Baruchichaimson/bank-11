#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "sll.h"

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_MESSAGES  10

sll_t *queue;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;                  
int consumed_count = 0;      
int finished_producing = 0;

int messages[NUM_PRODUCERS][NUM_MESSAGES];

/* ----------------- Producer ----------------- */
void *producer(void *arg)
{
    int id = (int)(size_t)arg;

    for (int i = 0; i < NUM_MESSAGES; i++)
    {
        messages[id][i] = id * 100 + i;

        pthread_mutex_lock(&lock);

        SLLInsert(SLLEnd(queue), &messages[id][i]);
        printf("Producer %d produced %d\n", id, messages[id][i]);

        pthread_mutex_unlock(&lock);

        sem_post(&sem); 
    }

    return NULL;
}

/* ----------------- Consumer ----------------- */
void *consumer(void *arg)
{
    int id = (int)(size_t)arg;
    int *msg = NULL;
    sll_iter_t it;

    while (1)
    {
        sem_wait(&sem); 
        pthread_mutex_lock(&lock);

        it = SLLBegin(queue);
        msg = (int*)SLLGetData(it);
        SLLRemove(it);
        consumed_count++;
        pthread_mutex_unlock(&lock);

        printf("Consumer %d consumed %d\n", id, *msg);

        if (finished_producing && consumed_count >= NUM_PRODUCERS * NUM_MESSAGES && SLLIsEmpty(queue))
        {
            pthread_mutex_unlock(&lock);
            break;
        }
    }

    return NULL;
}

/* ----------------- Main ----------------- */
int main()
{
    pthread_t prod[NUM_PRODUCERS];
    pthread_t cons[NUM_CONSUMERS];

    queue = SLLCreate();
    if (!queue) return 1;

    sem_init(&sem, 0, 0); 

    for (size_t i = 0; i < NUM_PRODUCERS; i++)
        pthread_create(&prod[i], NULL, producer, (void*)(size_t)i);

    for (size_t i = 0; i < NUM_CONSUMERS; i++)
        pthread_create(&cons[i], NULL, consumer, (void*)(size_t)i);

    for (size_t i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    finished_producing = 1;

    /*for (size_t i = 0; i < NUM_CONSUMERS; i++)
        sem_post(&sem);*/

    for (size_t i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    sem_destroy(&sem);
    pthread_mutex_destroy(&lock);
    SLLDestroy(queue);

    printf("All messages produced and consumed.\n");
    return 0;
}
