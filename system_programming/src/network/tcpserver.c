#include "network.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define PORT (5000)

typedef struct ClientArgs
{
    NetSocket client;
} ClientArgs;

static void *ClientHandler(void *arg)
{
    ClientArgs *args = (ClientArgs *)arg;
    NetSocket client = args->client;

    char buffer[NET_BUF_SIZE] = {0};
    char reply[128] = {0};
    int n = 0;

    free(args); 

    while (1)
    {
        n = NetReceive(&client, buffer, NULL);
        if (n <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        printf("Server received: %s\n", buffer);
        sprintf(reply, "Pong: %s\n", buffer);
        NetSend(&client, reply, NULL);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    ClientArgs *args = NULL;
    NetSocket server = NetCreate(NET_TCP);
    NetBind(&server, PORT);
    NetListen(&server, 5);

    printf("TCP Server running on port %d ...\n", PORT);

    while (1)
    {
        NetSocket client;

        printf("Waiting for client...\n");
        NetAccept(&server, &client);
        printf("Client connected!\n");

        args = malloc(sizeof(ClientArgs));
        args->client = client;

        
        pthread_create(&tid, NULL, ClientHandler, args);
        pthread_detach(tid);  
    }

    return 0;
}
