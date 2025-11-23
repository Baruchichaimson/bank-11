#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include <string.h> /* strcmp */
#include <unistd.h>
#include <errno.h> /* perror */
#include <time.h> /* time_t */
#include <fcntl.h> /* NonBlocking */
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h> /* sockaddr_in */
#include <sys/select.h> /* select */
#include <netinet/in.h>

#define DEFAULT_PORT (5000)
#define BUF_SIZE (2048)
#define MAX_CLIENTS FD_SETSIZE 

typedef struct 
{
    int fd;
    char buf[BUF_SIZE];
    int buf_len;
} Client;

/************ declertion static functions ****************************/

static int set_nonblocking(int fd); 
static ssize_t send_all(int fd, const char *data, size_t len); 
static void make_pong(const char *in, char *out); 
static int find_newline(const char *buf, int len);

/**********************************************************************/

int main(int argc, char **argv) 
{
    int port = 0;
    int opt = 1;
    int tcp_listen = 0;
    int udp_sock = 0;
    struct sockaddr_in addr;
    fd_set readfds;
    Client clients[MAX_CLIENTS];
    int i = 0;
    int maxfd = 0;
    int ready = 0;
    time_t last_activity;
    struct timeval tv;

    port = (argc >= 2) ? atoi(argv[1]) : DEFAULT_PORT;

    for (i = 0; i < MAX_CLIENTS; ++i) 
    {
        clients[i].fd = -1;
    }

    tcp_listen = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_listen < 0) 
    { 
        perror("socket tcp"); 
        exit(1); 
    }
        
    setsockopt(tcp_listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) 
    { 
        perror("socket udp"); 
        exit(1); 
    }
 
    setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(tcp_listen, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
    { 
        perror("bind tcp"); 
        exit(1); 
    }
    if (bind(udp_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
    { 
        perror("bind udp"); 
        exit(1); 
    }

    if (listen(tcp_listen, 10) < 0) 
    { 
        perror("listen"); 
        exit(1); 
    }

    set_nonblocking(tcp_listen);
    set_nonblocking(udp_sock);
    set_nonblocking(STDIN_FILENO);

    printf("Server listening on port %d (TCP+UDP)\n", port);

    maxfd = (tcp_listen > udp_sock) ? tcp_listen : udp_sock;
    last_activity = time(NULL);

    while (1) 
    {
        FD_ZERO(&readfds);
        FD_SET(tcp_listen, &readfds);
        FD_SET(udp_sock, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        for (i = 0; i < MAX_CLIENTS; ++i) 
        {
            if (clients[i].fd != -1) 
            {
                FD_SET(clients[i].fd, &readfds);
                if (clients[i].fd > maxfd) 
                {
                    maxfd = clients[i].fd;
                }
            }
        }

        tv.tv_sec = 7;
        tv.tv_usec = 0;

        ready = select(maxfd + 1, &readfds, NULL, NULL, &tv);
        if (ready < 0) 
        {
            if (errno == EINTR) 
            {
                continue;
            }
            perror("select");
            break;
        }
        if (ready == 0) 
        {
            printf("[server] No activity for 7 seconds\n");
            continue;
        }

        last_activity = time(NULL);

        if (FD_ISSET(STDIN_FILENO, &readfds)) 
        {
            char line[256];
            ssize_t r;
            r = read(STDIN_FILENO, line, sizeof(line)-1);
            if (r > 0) 
            {
                char *p;
                line[r] = '\0';
                p = strchr(line, '\n'); 
                if (p)
                {
                     *p = '\0';
                }
                if (strcmp(line, "quit") == 0) 
                { 
                    printf("Quit command received. Shutting down.\n"); 
                    break; 
                }
                else
                {
                    if (strcmp(line, "ping") == 0) 
                    {
                        printf("pong\n");
                    }
                    else
                    {
                        printf("stdin: %s\n", line);
                    }
                } 
            }
        }

        if (FD_ISSET(tcp_listen, &readfds)) 
        {
            while (1) 
            {
                struct sockaddr_in cli_addr;
                socklen_t len = sizeof(cli_addr);
                int client_fd = accept(tcp_listen, (struct sockaddr*)&cli_addr, &len);
                int slot;
                if (client_fd < 0) 
                {
                    if (errno == EWOULDBLOCK || errno == EAGAIN) 
                    {
                        break;
                    }
                    perror("accept"); 
                    break;
                }
                set_nonblocking(client_fd);
                slot = -1;
                for (i = 0; i < MAX_CLIENTS; ++i)
                { 
                    if (clients[i].fd == -1) 
                    { 
                        slot = i; 
                        break; 
                    }
                }
                if (slot == -1) 
                { 
                    close(client_fd); 
                    fprintf(stderr,"Too many clients\n"); 
                }
                else 
                { 
                    clients[slot].fd = client_fd; 
                    clients[slot].buf_len = 0; 
                    printf("New TCP client fd=%d\n", client_fd); 
                }
            }
        }

        if (FD_ISSET(udp_sock, &readfds)) 
        {
            char buf[BUF_SIZE];
            struct sockaddr_in from;
            socklen_t flen = sizeof(from);
            ssize_t n, s;
            char reply[256];
            n = recvfrom(udp_sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&from, &flen);
            if (n > 0) 
            {
                buf[n] = '\0';
                {
                    char *nl = strchr(buf, '\n'); if (nl) *nl = '\0';
                }
                printf("UDP received from %s:%d: '%s'\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
                make_pong(buf, reply);
                s = sendto(udp_sock, reply, strlen(reply), 0, (struct sockaddr*)&from, flen);
                if (s < 0) 
                {
                    perror("sendto reply");
                }
                else 
                {
                    printf("UDP replied to %s:%d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
                }
            }
        }

        for (i = 0; i < MAX_CLIENTS; ++i) 
        {
            int fd, n, linelen;
            char line[BUF_SIZE], reply[256];
            if (clients[i].fd == -1) 
            {
                continue;
            }

            fd = clients[i].fd;
            if (!FD_ISSET(fd, &readfds)) 
            {
                continue;
            }

            n = recv(fd, clients[i].buf + clients[i].buf_len, sizeof(clients[i].buf) - clients[i].buf_len - 1, 0);
            if (n < 0) 
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN) continue;
                perror("recv client"); n = 0;
            }
            if (n == 0) 
            {
                printf("TCP client fd=%d disconnected\n", fd);
                close(fd);
                clients[i].fd = -1;
                clients[i].buf_len = 0;
                continue;
            }

            clients[i].buf_len += n;
            clients[i].buf[clients[i].buf_len] = '\0';

            while ((linelen = find_newline(clients[i].buf, clients[i].buf_len)) >= 0) 
            {
                memcpy(line, clients[i].buf, linelen);
                line[linelen] = '\0';
                memmove(clients[i].buf, clients[i].buf + linelen, clients[i].buf_len - linelen);
                clients[i].buf_len -= (linelen + 1);

                printf("TCP client fd=%d sent: '%s'\n", fd, line);
                make_pong(line, reply);
                send_all(fd, reply, strlen(reply));
                printf("Replied to TCP client fd=%d\n", fd);
            }
        }
    }

    for (i = 0; i < MAX_CLIENTS; ++i) 
    {
        if (clients[i].fd != -1) 
        {
            close(clients[i].fd);
        }
    }
    close(tcp_listen);
    close(udp_sock);
    printf("Server shutdown\n");
    return 0;
}


static int set_nonblocking(int fd) 
{
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) 
    {
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) 
    {
        return -1;
    }
    return 0;
}

static ssize_t send_all(int fd, const char *data, size_t len) 
{
    ssize_t sent;
    sent = send(fd, data, len, 0);
    return sent;
}

static void make_pong(const char *in, char *out) 
{
    if (in == NULL) 
    {
        sprintf(out, "Pong\n");
    } 
    else 
    {
        if ((int)strlen(in) > 1000) 
        {
            char tmp[1001];
            strncpy(tmp, in, 1000);
            tmp[1000] = '\0';
            sprintf(out, "Pong: %s\n", tmp);
        } 
        else 
        {
            sprintf(out, "Pong: %s\n", in);
        }
    }
}

static int find_newline(const char *buf, int len) {
    int i;
    for (i = 0; i < len; ++i) if (buf[i] == '\n') return i;
    return -1;
}

