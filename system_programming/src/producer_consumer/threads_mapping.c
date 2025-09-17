/***********************************
 Exercise: Multi Threaded Memory Mapping
 Date: 15/09/25
 Developer: Baruch Haimson
 Reviewer: 
 Status: Approved
***********************************/

#include <pthread.h> /* pthread_t */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#define NUM_THREADS (3)

int global_var = 99;

__thread int tls_var = 100;

void* thread_func(void* arg) 
{
    int local_var = 10; 
    int* heap_var = malloc(sizeof(int)); 
    *heap_var = 200;

    printf("Thread %ld stack var: %p\n", (long)arg, (void *)&local_var);
    printf("Thread %ld TLS var: %p\n",   (long)arg, (void *)&tls_var);
    printf("Thread %ld heap var: %p\n",  (long)arg, (void *)heap_var);
    printf("Global var: %p\n",           (void *)&global_var);

    free(heap_var);
    return NULL;
}

int main() 
{
    long i = 0;
    int j = 0;

    pthread_t threads[NUM_THREADS];

    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }

    for(j = 0; j < NUM_THREADS; j++)
    {
        pthread_join(threads[j], NULL);
    }

    return 0;
}
