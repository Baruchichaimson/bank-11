#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE (1024)

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

    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) 
    { 
        perror("shmget"); 
        exit(1); 
    }

    data = (char*) shmat(shmid, NULL, 0);
    if (data == (char*) -1) 
    { 
        perror("shmat"); 
        exit(1); 
    }

    sprintf(data, "Hello from P1 to shared memory!");

    printf("P1 wrote: %s\n", data);

    if (shmdt(data) == -1) 
    { 
        perror("shmdt"); 
        exit(1);
    }

    printf("P1 done. Exiting...\n");
    return 0;
}
