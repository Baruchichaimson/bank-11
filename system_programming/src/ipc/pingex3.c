#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO1 "/tmp/ping_to_pong"
#define FIFO2 "/tmp/pong_to_ping"
#define MSG_SIZE (20)
#define ROUNDS (5)

int main()
{
    char buffer[MSG_SIZE] = {0};
    int fd_write = 0;
    int fd_read = 0;
    int i = 0;

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    fd_write = open(FIFO1, O_WRONLY);
    fd_read  = open(FIFO2, O_RDONLY);

    for (i = 0; i < ROUNDS; ++i)
    {
        sprintf(buffer, "Ping %d", i + 1);
        write(fd_write, buffer, strlen(buffer) + 1);
        sleep(1);
        read(fd_read, buffer, MSG_SIZE);
        printf("PING received: %s\n", buffer);
    }

    close(fd_write);
    close(fd_read);

    return 0;
}
