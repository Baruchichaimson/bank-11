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
    int fd_read = 0;
    int fd_write = 0;
    int i = 0;

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    fd_read  = open(FIFO1, O_RDONLY);
    fd_write = open(FIFO2, O_WRONLY);

    for (i = 0; i < ROUNDS; ++i)
    {
        read(fd_read, buffer, MSG_SIZE);
        printf("PONG received: %s\n", buffer);
        sleep(1);
        sprintf(buffer, "Pong %d", i + 1);
        write(fd_write, buffer, strlen(buffer) + 1);
    }

    close(fd_read);
    close(fd_write);

    return 0;
}
