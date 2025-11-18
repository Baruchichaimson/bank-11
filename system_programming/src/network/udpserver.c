#include "network.h"
#include <stdio.h>
#include <unistd.h>

#define PORT (6000)

int main()
{
    char reply[128] = {0};
    char buffer[NET_BUF_SIZE] = {0};
    struct sockaddr_in client_addr;

    NetSocket server = NetCreate(NET_UDP);
    NetBind(&server, PORT);

    while (1)
    {
        NetReceive(&server, buffer, &client_addr);
        printf("Pong received: %s\n", buffer);

        
        sprintf(reply, "Pong: %s", buffer);

        NetSend(&server, reply, &client_addr);
    }

    return 0;
}
