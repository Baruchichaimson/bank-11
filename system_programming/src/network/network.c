#include "network.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

NetSocket NetCreate(NetType type)
{
    NetSocket ns = {0};
    int socktype = 0;
    ns.type = type;

    if(type == NET_TCP)
    {
       socktype = SOCK_STREAM;
    }
    else
    {
        socktype = SOCK_DGRAM;
    }

    ns.sockfd = socket(AF_INET, socktype, 0);
    if (ns.sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    return ns;
}

void NetInitAddr(struct sockaddr_in *addr, const char *ip, int port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr->sin_addr);
}

int NetBind(NetSocket *ns, int port)
{
    struct sockaddr_in addr;

    NetInitAddr(&addr, "0.0.0.0", port);

    if (bind(ns->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    return 0;
}

int NetListen(NetSocket *ns, int backlog)
{
    if (ns->type != NET_TCP)
    {
        fprintf(stderr, "Listen not allowed for UDP\n");
        return -1;
    }

    if (listen(ns->sockfd, backlog) < 0)
    {
        perror("listen");
        exit(1);
    }
    return 0;
}

int NetAccept(NetSocket *ns, NetSocket *out)
{
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int client_sock = accept(ns->sockfd, (struct sockaddr *)&client_addr, &len);

    if (ns->type != NET_TCP)
    {
        fprintf(stderr, "Accept not allowed for UDP\n");
        return -1;
    }

    if (client_sock < 0)
    {
        perror("accept");
        exit(1);
    }

    out->sockfd = client_sock;
    out->type = NET_TCP;

    return 0;
}

int NetConnect(NetSocket *ns, const char *ip, int port)
{
    struct sockaddr_in addr;

    if (ns->type != NET_TCP)
    {
        fprintf(stderr, "Connect not allowed for UDP\n");
        return -1;
    }
    
    NetInitAddr(&addr, ip, port);

    if (connect(ns->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    return 0;
}

int NetSend(NetSocket *ns, const char *msg, struct sockaddr_in *dest)
{
    if (ns->type == NET_TCP)
    {
        return send(ns->sockfd, msg, strlen(msg), 0);
    }
    else
    {
        return sendto(ns->sockfd, msg, strlen(msg), 0, (struct sockaddr *)dest, sizeof(*dest));
    }
}

int NetReceive(NetSocket *ns, char *buffer, struct sockaddr_in *src)
{
    int n;

    if (ns->type == NET_TCP)
    {
        n = recv(ns->sockfd, buffer, NET_BUF_SIZE - 1, 0);
    }
    else
    {
        socklen_t len = sizeof(*src);
        n = recvfrom(ns->sockfd, buffer, NET_BUF_SIZE - 1, 0, (struct sockaddr *)src, &len);
    }

    if (n >= 0)
    {
        buffer[n] = '\0';                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   }

    return n;
}
