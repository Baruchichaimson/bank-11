#include <unistd.h>
#include <stdio.h>

#include "network.h"

#define SERVER_PORT (5000)

int main()
{
    int i = 1;
    char msg[128] = {0};
    char buffer[NET_BUF_SIZE] = {0};
    NetSocket client = NetCreate(NET_UDP);

    struct sockaddr_in addr;
    NetInitAddr(&addr, "127.0.0.1", SERVER_PORT);

    for (i = 1; i <= 5; ++i)
    {
        
        sprintf(msg, "Ping %d", i);
        NetSend(&client, msg, &addr);

        NetReceive(&client, buffer, NULL);
        printf("Ping received: %s\n", buffer);

        sleep(1);
    }

    return 0;
}
