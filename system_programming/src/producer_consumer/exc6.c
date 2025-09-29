#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CONSUMERS 3
#define NUM_MESSAGES 5

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int message = 0;                         
int consumers_done = 0;                  
int consumed[NUM_CONSUMERS] = {0};  
int total_consumers = NUM_CONSUMERS;
int finished = 0;

/* ----------------- Consumer ----------------- */
void* consumer(void* arg)
{
    int id = (int)(size_t)arg;

    while (1)
    {
        pthread_mutex_lock(&lock);

        while ((message == 0 || consumed[id]) && !finished)
        {
            pthread_cond_wait(&cond, &lock);
        }

        if (finished)
        {
            pthread_mutex_unlock(&lock);
            break;
        }

        printf("Consumer %d consumed message %d\n", id, message);
        consumed[id] = 1;
        consumers_done++;

        if (consumers_done == total_consumers)
        {
            pthread_cond_signal(&cond);
        }

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

/* ----------------- Producer ----------------- */
void* producer(void* arg)
{
    int i = 1;
    int j = 0;

    for (i = 1; i <= NUM_MESSAGES; i++)
    {
        pthread_mutex_lock(&lock);

        message = i;
        consumers_done = 0;
        for (j = 0; j < total_consumers; j++)
        {
            consumed[j] = 0;
        }

        printf("Producer produced message %d\n", message);

        pthread_cond_broadcast(&cond);

        while (consumers_done < total_consumers)
        {
            pthread_cond_wait(&cond, &lock);
        }

        pthread_mutex_unlock(&lock);

        sleep(1);
    }

    pthread_mutex_lock(&lock);
    finished = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);

    printf("Producer finished producing.\n");
    return NULL;
}

/* ----------------- Main ----------------- */
int main()
{
    int i = 0;
    pthread_t prod;
    pthread_t cons[NUM_CONSUMERS];

    pthread_create(&prod, NULL, producer, NULL);

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_create(&cons[i], NULL, consumer, (void*)(size_t)i);
    }

    pthread_join(prod, NULL);
    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(cons[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    printf("All messages were produced and consumed.\n");

    return 0;
}
