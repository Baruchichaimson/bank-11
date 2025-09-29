#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define ALPHABET_SIZE 256 
#define MAX_THREADS 16

typedef struct ThreadArgs
{
    const char *data;
    size_t start;
    size_t end;
    int *local_count;

} thread_arg_t;

void *count_worker(void *arg) 
{
    size_t i = 0;
    unsigned char c = 0;
    thread_arg_t *t = (thread_arg_t *)arg;

    for (i = t->start; i < t->end; i++) 
    {
        c = (unsigned char)t->data[i];
        t->local_count[c]++;
    }
    return NULL;
}

void MTCountingSort(const char *input, size_t n, char *output, size_t num_threads) 
{
    size_t global_count[ALPHABET_SIZE] = {0};
    thread_arg_t args[MAX_THREADS];
    pthread_t threads[MAX_THREADS];
    int *local_counts[MAX_THREADS];
    size_t chunk = 0;
    size_t pos = 0;
    size_t t = 0;
    size_t j = 0;
    size_t c = 0;
    size_t k = 0;

    if (num_threads > MAX_THREADS) 
    {
        num_threads = MAX_THREADS;
    }

    for (t = 0; t < num_threads; t++) 
    {
        local_counts[t] = calloc(ALPHABET_SIZE, sizeof(int));
        if(!local_counts[t])
        {
            printf("Error: failed to allocate memory for local_counts[%ld]\n", t);
            for (j = 0; j < t; j++) 
            {
                free(local_counts[j]);
            }
            exit(1);
        }
    }

    chunk = n / num_threads;

    for (t = 0; t < num_threads; t++) 
    {
        args[t].data = input;
        args[t].start = t * chunk;
        args[t].end = (t == num_threads - 1) ? n : (t + 1) * chunk;
        args[t].local_count = local_counts[t];

        pthread_create(&threads[t], NULL, count_worker, &args[t]);
    }

    for (t = 0; t < num_threads; t++) 
    {
        pthread_join(threads[t], NULL);
    }

    for (t = 0; t < num_threads; t++) 
    {
        for (c = 0; c < ALPHABET_SIZE; c++) 
        {
            global_count[c] += local_counts[t][c];
        }
        free(local_counts[t]);
    }


    for (c = 0; c < ALPHABET_SIZE; c++) 
    {
        for (k = 0; k < global_count[c]; k++) 
        {
            output[pos++] = (char)c;
        }
    }
}

int main(int argc, char **argv) 
{
    FILE *f = NULL;
    char *sorted = NULL;
    char *big_buffer = NULL;
    char *buffer = NULL;
    clock_t start_time;
    clock_t end_time;
    size_t n = 0;
    size_t copies = 10;
    size_t big_n = 0;
    size_t i = 0;
    size_t num_threads = 0;

    if (argc < 3) 
    {
        printf("Add <dictionary_file> and <num_threads>\n");
        return 1;
    }

    num_threads = atoi(argv[2]);

    f = fopen(argv[1], "r");
    if (!f) 
    {
        perror("fopen");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    n = ftell(f);
    rewind(f);

    buffer = malloc(n);
    if(!buffer)
    {
        return 1;
    }

    fread(buffer, 1, n, f);
    fclose(f);

    big_n = n * copies;
    big_buffer = malloc(big_n);
    if(!big_buffer)
    {
        free(buffer);
        return 1;
    }

    for (i = 0; i < copies; i++) 
    {
        memcpy(big_buffer + i * n, buffer, n);
    }

    free(buffer);

    printf("=== Original Dictionary ===\n");
    fwrite(big_buffer, 1, big_n, stdout);
    printf("\n\n");

    sorted = malloc(big_n);
    if(!sorted)
    {
        return 1;
    }

    start_time = clock();

    MTCountingSort(big_buffer, big_n, sorted, num_threads);

    end_time = clock();

    printf("=== Sorted Letters (A-Z, a-z) ===\n");
    for (size_t i = 0; i < big_n; i++) 
    {
        if ((sorted[i] >= 'A' && sorted[i] <= 'Z'))
        {
            putchar(sorted[i]);
        }
    }
    printf("\n");

    printf("Time elapsed: %.3f seconds\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC);

    free(big_buffer);
    free(sorted);

    return 0;
}
