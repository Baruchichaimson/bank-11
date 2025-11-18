#include "network.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROADCAST_PORT (5000)

int main()
{
    int i = 1;
    int enable = 1;
    char msg[64] = {0};
    struct sockaddr_in broadcast;
    NetSocket sender = NetCreate(NET_UDP);

    setsockopt(sender.sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
    NetInitAddr(&broadcast, "255.255.255.255", BROADCAST_PORT);

    for (i = 1; i <= 5; ++i)
    {
        sprintf(msg, "baruchi %d", i);
        NetSend(&sender, msg, &broadcast);
        printf("Broadcast: %s\n", msg);
        sleep(1);
    }

    return 0;
}
