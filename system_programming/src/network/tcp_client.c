
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define BUF_SIZE 2048

static int recv_line(int fd, char *out, int maxlen) 
{
    int idx = 0;
    while (1) 
    {
        ssize_t r = recv(fd, out + idx, 1, 0);
        if (r <= 0) 
        {
            return r;
        }
        if (out[idx] == '\n') 
        { 
            out[idx] = '\0'; 
            return idx+1; 
        }

        idx++;

        if (idx >= maxlen-1) 
        { 
            out[idx] = '\0'; 
            return idx; 
        }
    }
}

int main(int argc, char **argv) 
{
    const char *ip;
    int port;
    int count;
    int sock;
    struct sockaddr_in saddr;
    char buf[BUF_SIZE];
    int i, r, wait;

    if (argc < 3) 
    { 
        fprintf(stderr,"Usage: %s <server-ip> <port>\n", argv[0]); 
        return 1; 
    }
    ip = argv[1];
    port = atoi(argv[2]);

    srand(time(NULL) ^ getpid());
    count = 3 + rand()%8;
    printf("[tcp_client] will send %d pings\n", count);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    { 
        perror("socket"); 
        return 1; 
    }

    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &saddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) 
    {
        perror("connect");
        close(sock);
        return 1;
    }
    printf("[tcp_client] connected to %s:%d\n", ip, port);

    for (i = 1; i <= count; ++i) 
    {
        sprintf(buf, "Ping %d\n", i);
        printf("[tcp_client] sending: %s", buf);
        if (send(sock, buf, strlen(buf), 0) < 0) 
        { 
            perror("send"); 
            break; 
        }

        r = recv_line(sock, buf, sizeof(buf));
        if (r <= 0) 
        { 
            printf("[tcp_client] server closed or error\n"); 
            break; 
        }
        printf("[tcp_client] received: %s\n", buf);

        wait = 3 + rand()%8; 
        printf("[tcp_client] sleeping %d seconds\n", wait);
        sleep(wait);
    }

    printf("[tcp_client] done, closing\n");
    close(sock);
    return 0;
}
