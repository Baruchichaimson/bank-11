#include "network.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROADCAST_PORT (5000)

int main()
{
    int i = 1;
    int n = 0;
    int enable = 1;
    char buf[64] = {0};
    char msg[64] = {0};
    socklen_t fromlen;
    struct sockaddr_in broadcast;
    struct sockaddr_in from;
    NetSocket sender = NetCreate(NET_UDP);

    setsockopt(sender.sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
    NetInitAddr(&broadcast, "255.255.255.255", BROADCAST_PORT);

    for (i = 1; i <= 5; ++i)
    {
        sprintf(msg, "ping"); 
        NetSend(&sender, msg, &broadcast);
        printf("Broadcast sent: %s\n", msg);

        fromlen = sizeof(from);
        n = recvfrom(sender.sockfd, buf, sizeof(buf)-1, 0,
                        (struct sockaddr*)&from, &fromlen);
        if (n > 0) 
        {
            buf[n] = '\0';
            printf("Received pong from %s\n", inet_ntoa(from.sin_addr));
        }

        sleep(rand() % 8 + 3);
    }

    return 0;
}
