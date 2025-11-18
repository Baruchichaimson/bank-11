/**************************************
    reader.c  (P2/P3/P4)
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MSG_TEXT_SIZE 64

struct msgbuf
{
    long mtype;
    char mtext[MSG_TEXT_SIZE];
};

int main()
{
    struct msgbuf message;
    int msgid = 0;

    key_t key = ftok("msgqueuefile", 65);
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    msgid = msgget(key, 0666);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }


    while (1)
    {
        if (msgrcv(msgid, &message, sizeof(message.mtext), 0, IPC_NOWAIT) == -1)
        {
            if (errno == ENOMSG)
            {
                printf("No more messages. Reader exiting.\n");
                break;
            }
            perror("msgrcv");
            exit(1);
        }

        printf("Reader(pid %d) received: %s\n", getpid(), message.mtext);
        sleep(1);
    }

    return 0;

}
