/*****************
 Exercise:	FSQ
 Date:		14/09/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:  Aproved
 *****************/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

#include "fsq.h"

#define DEFAULT_CAP  (4)
#define MAX(a,b)	(((a) > (b)) ? (a) : (b))


struct fsq
{
    size_t read_index;
    size_t write_index;
    size_t capacity;
    pthread_mutex_t read_lock;
    pthread_mutex_t write_lock;
    sem_t con_sem;
    sem_t pro_sem;
    void* buffer[1];
};

/* ----------------- fsq Create / Destroy ----------------- */
fsq_t* FSQCreate(size_t capacity)
{
	fsq_t* fsq;
    size_t cap;

    if (capacity == 0)
        cap = DEFAULT_CAP;
    else
        cap = MAX(DEFAULT_CAP, capacity);

    fsq = (fsq_t*)malloc(sizeof(fsq_t) + (cap - 1) * sizeof(void*));
    if (!fsq) return NULL;

    fsq->read_index = 0;
    fsq->write_index = 0;
    fsq->capacity = cap;

    if (pthread_mutex_init(&fsq->read_lock, NULL)) 
    { 
        free(fsq); 
        return NULL; 
    }
    if (pthread_mutex_init(&fsq->write_lock, NULL)) 
    { 
        pthread_mutex_destroy(&fsq->read_lock); 
        free(fsq); 
        return NULL; 
    }

    if (sem_init(&fsq->con_sem, 0, cap)) 
    { 
        pthread_mutex_destroy(&fsq->read_lock); 
        pthread_mutex_destroy(&fsq->write_lock); 
        free(fsq); 
        return NULL; 
    }
    if (sem_init(&fsq->pro_sem, 0, 0)) 
    { 
        sem_destroy(&fsq->con_sem); 
        pthread_mutex_destroy(&fsq->read_lock); 
        pthread_mutex_destroy(&fsq->write_lock); 
        free(fsq); 
        return NULL; 
    }

    return fsq;
}

void FSQDestroy(fsq_t *fsq)
{
    assert(fsq);

    sem_destroy(&fsq->con_sem);
    sem_destroy(&fsq->pro_sem);
    pthread_mutex_destroy(&fsq->read_lock);
    pthread_mutex_destroy(&fsq->write_lock);

    free(fsq);
}

/* ----------------- fsq Enfsqueue ----------------- */
void FSQEnqueue(fsq_t *fsq, void* value)
{
    sem_wait(&fsq->con_sem);                
    pthread_mutex_lock(&fsq->write_lock);

    fsq->buffer[fsq->write_index % fsq->capacity] = value;
    ++fsq->write_index;

    pthread_mutex_unlock(&fsq->write_lock);
    sem_post(&fsq->pro_sem);               
}

/* ----------------- fsq Defsqueue ----------------- */
void* FSQDequeue(fsq_t *fsq)
{
    void* value = NULL;

    sem_wait(&fsq->pro_sem);                
    pthread_mutex_lock(&fsq->read_lock);

    value = fsq->buffer[fsq->read_index % fsq->capacity];
    ++fsq->read_index;

    pthread_mutex_unlock(&fsq->read_lock);
    sem_post(&fsq->con_sem);                

    return value;
}
