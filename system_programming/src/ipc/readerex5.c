#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() 
{
    int shmid = 0;
    char *data = NULL;
    key_t key = ftok("shmfile", 65);
    if (key == -1) 
    { 
        perror("ftok"); 
        exit(1); 
    }

    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) 
    { 
        perror("shmget"); 
        exit(1); 
    }

    data = (char*) shmat(shmid, NULL, SHM_RDONLY);
    if (data == (char*) -1) 
    { 
        perror("shmat"); 
        exit(1); 
    }

    printf("Reader(pid %d) read: %s\n", getpid(), data);

    if (shmdt(data) == -1) 
    { 
        perror("shmdt"); 
        exit(1); 
    }

    return 0;
}
