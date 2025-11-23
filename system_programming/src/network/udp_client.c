#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define BUF_SIZE 2048

int main(int argc, char **argv) 
{
    const char *ip;
    int port;
    int count;
    int sock;
    struct sockaddr_in saddr;
    char buf[BUF_SIZE];
    int i, wait;
    struct timeval tv;
    struct sockaddr_in from;
    socklen_t fl;
    int yes = 1;
    ssize_t r;

    if (argc < 3) 
    {
        fprintf(stderr,"Usage: %s <server-ip> <port>\n", argv[0]);
        return 1;
    }

    ip = argv[1];
    port = atoi(argv[2]);

    srand(time(NULL) ^ getpid());
    count = 3 + rand()%8; 
    printf("[udp_client] will send %d pings\n", count);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
    { 
        perror("socket"); 
        return 1; 
    }

    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &saddr.sin_addr);

    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));

    for (i = 1; i <= count; ++i) 
    {
        sprintf(buf, "Ping %d\n", i);
        printf("[udp_client] sending: %s", buf);
        if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) 
        {
            perror("sendto");
            break;
        }

        fl = sizeof(from);
        r = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&from, &fl);
        if (r <= 0) 
        {
            perror("recvfrom");
            printf("[udp_client] no reply (timeout or error). continuing\n");
        } 
        else 
        {
            buf[r] = '\0';
            printf("[udp_client] received from %s:%d : %s\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
        }

        wait = 3 + rand()%8;
        printf("[udp_client] sleeping %d seconds\n", wait);
        sleep(wait);
    }

    printf("[udp_client] done, exiting\n");
    close(sock);
    return 0;
}
