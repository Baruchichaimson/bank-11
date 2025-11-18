#include "network.h"
#include <stdio.h>

#define BROADCAST_PORT (7000)

int main()
{
    char buffer[NET_BUF_SIZE] = {0};
    struct sockaddr_in src = {0};

    NetSocket receiver = NetCreate(NET_UDP);
    NetBind(&receiver, BROADCAST_PORT);

    while (1)
    {
        NetReceive(&receiver, buffer, &src);
        printf("Received broadcast: %s\n", buffer);
    }

    return 0;
}
