#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "fsq.h"

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_MESSAGES  10
#define QUEUE_CAPACITY 5

fsq_t *queue;

/* ----------------- Producer ----------------- */
void *producer(void *arg)
{
    int i = 0;
    int id = (int)(size_t)arg;
    for (i = 0; i < NUM_MESSAGES; i++)
    {
        int msg = id * 100 + i;
        FSQEnqueue(queue, (void*)(size_t)msg);
        printf("Producer %d produced %d\n", id, msg);
        usleep(rand() % 100000); 
    }
    return NULL;
}

/* ----------------- Consumer ----------------- */
void *consumer(void *arg)
{
    int i = 0;
    int id = (int)(size_t)arg;
    
    for (i = 0; i < NUM_MESSAGES; i++)
    {
        int msg = (int)(size_t)FSQDequeue(queue);
        printf("Consumer %d consumed %d\n", id, msg);
        usleep(rand() % 150000); 
    }
    return NULL;
}

/* ----------------- Main ----------------- */
int main(void)
{
    size_t i = 0;

    pthread_t prod[NUM_PRODUCERS];
    pthread_t cons[NUM_CONSUMERS];

    srand((unsigned int)time(NULL));
    
    queue = FSQCreate(QUEUE_CAPACITY);
    if (!queue) return 1;

    for (i = 0; i < NUM_PRODUCERS; i++)
        pthread_create(&prod[i], NULL, producer, (void*)(size_t)i);

    for (i = 0; i < NUM_CONSUMERS; i++)
        pthread_create(&cons[i], NULL, consumer, (void*)(size_t)i);

    for (i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    for (i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    FSQDestroy(queue);
    printf("All messages produced and consumed.\n");
    return 0;
}
