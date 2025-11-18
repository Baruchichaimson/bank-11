#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <netinet/in.h>

#define NET_BUF_SIZE (1024)

typedef enum
{
    NET_TCP,
    NET_UDP
} NetType;

typedef struct
{
    int sockfd;
    NetType type;

} NetSocket;

NetSocket NetCreate(NetType type);
int NetBind(NetSocket *ns, int port);
int NetListen(NetSocket *ns, int backlog);    
int NetAccept(NetSocket *ns, NetSocket *out);  
int NetConnect(NetSocket *ns, const char *ip, int port); 

int NetSend(NetSocket *ns, const char *msg, struct sockaddr_in *dest);
int NetReceive(NetSocket *ns, char *buffer, struct sockaddr_in *src);

void NetInitAddr(struct sockaddr_in *addr, const char *ip, int port);

#endif /* __NETWORK_H__ */
