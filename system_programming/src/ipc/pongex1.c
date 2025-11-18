/******************************************************
 * pong.c
 * The "pong" process.
 * gcc pong.c -o pong -pthread
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SEM_PING "/sem_ping_example"
#define SEM_PONG "/sem_pong_example"

int main()
{
    int i = 0;
    sem_t *sem_ping = sem_open(SEM_PING, 1);
    sem_t *sem_pong = sem_open(SEM_PONG, 0);

    if (sem_ping == SEM_FAILED || sem_pong == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    for (i = 0; i < 5; i++)
    {
        sem_wait(sem_pong);
        printf("PONG %d\n", i);
        fflush(stdout);
        sem_post(sem_ping);
        sleep(1);
    }

    sem_close(sem_ping);
    sem_close(sem_pong);

    return 0;
}
