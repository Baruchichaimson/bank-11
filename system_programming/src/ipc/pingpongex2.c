#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#define MSG_SIZE (20)
#define ROUNDS (5)

int main() 
{
    int i = 0;
    pid_t pid = -1;
    
    int p2c[2] = {0};  
    int c2p[2] = {0};

    char buffer[MSG_SIZE];

    if (pipe(p2c) == -1 || pipe(c2p) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }   

    pid = fork();
    if (pid < 0) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    

    if (pid == 0) 
    {
        close(p2c[1]);  
        close(c2p[0]);  

        for (i = 0; i < ROUNDS; i++)
        {
            read(p2c[0], buffer, MSG_SIZE);
            printf("Child received: %s\n", buffer);
            sleep(1);
            sprintf(buffer, "Pong %d", i + 1);
            write(c2p[1], buffer, strlen(buffer) + 1);
        }
    }
    else  
    {
        close(p2c[0]);  
        close(c2p[1]);  

        for (i = 0; i < ROUNDS; i++)
        {
            sprintf(buffer, "Ping %d", i + 1);
            write(p2c[1], buffer, strlen(buffer) + 1);
            sleep(1);
            read(c2p[0], buffer, MSG_SIZE);
            printf("Parent received: %s\n", buffer);
        }
    }

    return 0;
}
