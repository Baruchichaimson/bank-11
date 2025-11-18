#include "network.h"
#include <stdio.h>
#include <unistd.h>

#define PORT (5000)

int main()
{
    int i = 1;
    char msg[128] = {0};
    char buffer[NET_BUF_SIZE] = {0};
    NetSocket client = NetCreate(NET_TCP);
    NetConnect(&client, "127.0.0.1", PORT);

    for (i = 1; i <= 5; ++i)
    {
        sprintf(msg, "Ping %d", i);
        NetSend(&client, msg, NULL);

        NetReceive(&client, buffer, NULL);
        printf("Client received: %s\n", buffer);

        sleep(1);
    }

    return 0;
}
