#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_MESSAGES 30

/* ---------------- Shared resources ---------------- */
static int message = 0;  /* ההודעה שמועברת בין producer ל-consumer */
static int ready = 0;    /* 0 = producer יכול לכתוב, 1 = consumer יכול לקרוא */
static int lock = 0;     /* spin lock */

/* ---------------- Test-and-Set / Spinlock ---------------- */
int test_and_set(int *lock_ptr)
{
    int old = *lock_ptr;
    *lock_ptr = 1;
    return old;
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

/* ---------------- Producer ---------------- */
void *producer(void *arg)
{
    int i;
    for (i = 0; i < NUM_MESSAGES; i++)
    {
        int done = 0;
        while (!done)
        {
            spin_lock(&lock);
            if (ready == 0)       /* אפשר לכתוב */
            {
                message = rand() % 100;
                printf("Producer produced: %d\n", message);
                ready = 1;        /* הודעה מוכנה לצרכן */
                done = 1;
            }
            spin_unlock(&lock);
        }
    }
    return NULL;
}

/* ---------------- Consumer ---------------- */
void *consumer(void *arg)
{
    int i;
    for (i = 0; i < NUM_MESSAGES; i++)
    {
        int done = 0;
        while (!done)
        {
            spin_lock(&lock);
            if (ready == 1)       /* אפשר לקרוא */
            {
                printf("Consumer consumed: %d\n", message);
                ready = 0;        /* הודעה נצרכה, producer יכול לכתוב */
                done = 1;
            }
            spin_unlock(&lock);
        }
    }
    return NULL;
}

/* ---------------- Main ---------------- */
int main(void)
{
    pthread_t prod, cons;

    srand((unsigned int)time(NULL));

    /* יצירת threads */
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

    /* המתנה לסיום threads */
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
