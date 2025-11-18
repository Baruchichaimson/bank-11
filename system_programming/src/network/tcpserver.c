#include "network.h"
#include <stdio.h>
#include <unistd.h>
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

    free(args); 

    char buffer[NET_BUF_SIZE] = {0};
    char reply[128] = {0};
    int n = 0;

    while (1)
    {
        n = NetReceive(&client, buffer, NULL);
        if (n <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        printf("Server received: %s\n", buffer);

        snprintf(reply, sizeof(reply), "Pong: %s", buffer);
        NetSend(&client, reply, NULL);
    }

    NetClose(&client);
    return NULL;
}

int main()
{
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

        ClientArgs *args = malloc(sizeof(ClientArgs));
        args->client = client;

        pthread_t tid;
        pthread_create(&tid, NULL, ClientHandler, args);
        pthread_detach(tid);  
    }

    NetClose(&server);
    return 0;
}
