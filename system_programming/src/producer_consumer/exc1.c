#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_MESSAGES 30

static int message = 0; 
static int ready = 0;    
static int lock = 0;   

int test_and_set(int *lock_ptr)
{
    return __sync_lock_test_and_set(lock_ptr, 1);
}

void spin_lock(int *lock_ptr)
{
    while (test_and_set(lock_ptr))
    { 
        /* busy wait */
    }
}

void spin_unlock(int *lock_ptr)
{
    *lock_ptr = 0;
}

void *producer()
{
    int i;
    int done = 0;

    for (i = 0; i < NUM_MESSAGES; i++)
    {
        done = 0;
        while (!done)
        {
            spin_lock(&lock);
            if (ready == 0)       
            {
                message = rand() % 100;
                printf("Producer produced: %d\n", message);
                ready = 1;      
                done = 1;
            }
            spin_unlock(&lock);
        }
    }
    return NULL;
}

void *consumer()
{
    int i;
    int done = 0;
    for (i = 0; i < NUM_MESSAGES; i++)
    {
        done = 0;
        while (!done)
        {
            spin_lock(&lock);
            if (ready == 1)       
            {
                printf("Consumer consumed: %d\n", message);
                ready = 0;        
                done = 1;
            }
            spin_unlock(&lock);
        }
    }
    return NULL;
}

int main(void)
{
    pthread_t prod, cons;


    if (pthread_create(&prod, NULL, producer, NULL) != 0)
    {
        printf("Error creating producer thread\n");
        return 1;
    }

    if (pthread_create(&cons, NULL, consumer, NULL) != 0)
    {
        printf("Error creating consumer thread\n");
        return 1;
    }

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
