#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define NUM_MSGS (10)
#define MSG_TEXT_SIZE (64)

struct msgbuf
{
    long mtype;
    char mtext[MSG_TEXT_SIZE];
};

int main()
{
    int i = 1;
    struct msgbuf message;
    int msgid =0;
    key_t key = ftok("msgqueuefile", 65);
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }


    for ( i = 1; i <= NUM_MSGS; i++)
    {
        message.mtype = 1;
        sprintf(message.mtext, "Hello from P1 message %d", i);

        if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }

        printf("P1 sent: %s\n", message.mtext);
        sleep(2);
    }

    printf("P1 done. Exiting...\n");
    return 0;  
}
