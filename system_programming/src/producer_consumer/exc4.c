#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "cbuff.h"

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_MESSAGES 10
#define CB_CAPACITY (NUM_PRODUCERS * NUM_MESSAGES * sizeof(int))

struct cbuff* buff;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t full;   
sem_t empty;  
int finished_producing = 0;

int messages[NUM_PRODUCERS][NUM_MESSAGES];

/* ---------------- Producer ---------------- */
void* producer(void* arg)
{
    int id = (int)(size_t)arg;

    for (size_t i = 0; i < NUM_MESSAGES; i++)
    {
        messages[id][i] = id * 100 + i;

        sem_wait(&empty);             
        pthread_mutex_lock(&lock);     

        CBuffWrite(buff, &messages[id][i], sizeof(int));
        printf("Producer %d produced %d\n", id, messages[id][i]);

        __sync_synchronize();
        pthread_mutex_unlock(&lock);
        sem_post(&full);              
    }

    return NULL;
}

/* ---------------- Consumer ---------------- */
void* consumer(void* arg)
{
    int id = (int)(size_t)arg;
    int msg = 0;

    while (1)
    {
        sem_wait(&full);               
        pthread_mutex_lock(&lock);

        if (finished_producing && CBuffIsEmpty(buff))
        {
            pthread_mutex_unlock(&lock);
            break;  
        }

        CBuffRead(buff, &msg, sizeof(int));
        printf("Consumer %d consumed %d\n", id, msg);

        __sync_synchronize();
        pthread_mutex_unlock(&lock);
        sem_post(&empty);
    }

    return NULL;
}

/* ---------------- Main ---------------- */
int main()
{
    pthread_t prod[NUM_PRODUCERS];
    pthread_t cons[NUM_CONSUMERS];

    buff = CBuffCreate(CB_CAPACITY);
    if (!buff) 
    {
        return 1;
    }

    if(sem_init(&full, 0, 0) == -1)
    {
        printf("FAILED MALLOC\n");
        return 0;        
    }   
    if(sem_init(&empty, 0, CB_CAPACITY) == -1)
    {
        printf("FAILED MALLOC\n");
        return 0;        
    }   

    for (size_t i = 0; i < NUM_PRODUCERS; i++)
        pthread_create(&prod[i], NULL, producer, (void*)(size_t)i);

    for (size_t i = 0; i < NUM_CONSUMERS; i++)
        pthread_create(&cons[i], NULL, consumer, (void*)(size_t)i);

    for (size_t i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    finished_producing = 1;

    for (size_t i = 0; i < NUM_CONSUMERS; i++)
        sem_post(&full);

    for (size_t i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&lock);
    CBuffDestroy(buff);

    printf("All messages produced and consumed.\n");
    return 0;
}
