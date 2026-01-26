#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define ROWS 4
#define COLS 4

int matrix[ROWS][COLS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_lock = PTHREAD_MUTEX_INITIALIZER;

int readers_count = 0;

void* reader(void* arg)
{
    int i = 0;
    int j = 0;
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);
    readers_count++;

    if (readers_count == 1)
    {
        pthread_mutex_lock(&write_lock);
    }

    pthread_mutex_unlock(&mutex);

    printf("Reader %d is reading:\n", id);

    for (i = 0; i < ROWS; ++i)
    {
        for (j = 0; j < COLS; ++j)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    sleep(1);

    pthread_mutex_lock(&mutex);
    readers_count--;

    if (readers_count == 0)
    {
        pthread_mutex_unlock(&write_lock);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}


void* writer(void* arg)
{
    int r = 0;
    int c = 0;
    int id = *(int*)arg;

    pthread_mutex_lock(&write_lock);

    r = id % ROWS;
    c = id % COLS;

    printf("Writer %d is writing to [%d][%d]\n", id, r, c);
    matrix[r][c] = id * 10;

    sleep(1);

    pthread_mutex_unlock(&write_lock);

    return NULL;
}

int main()
{
    int i = 0;
    int j = 0;

    pthread_t readers[5];
    pthread_t writers[3];

    int r_ids[5] = {1, 2, 3, 4, 5};
    int w_ids[3] = {1, 2, 3};

    for (i = 0; i < ROWS; ++i)
        for (j = 0; j < COLS; ++j)
            matrix[i][j] = 0;

    for (i = 0; i < 3; ++i)
        pthread_create(&writers[i], NULL, writer, &w_ids[i]);

    for (i = 0; i < 5; ++i)
        pthread_create(&readers[i], NULL, reader, &r_ids[i]);

    

    for (i = 0; i < 5; ++i)
        pthread_join(readers[i], NULL);

    for (i = 0; i < 3; ++i)
        pthread_join(writers[i], NULL);

    return 0;
}
