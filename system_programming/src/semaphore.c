#include <stdio.h>    /* printf */
#include <stdlib.h>   /* atexit */
#include <string.h>   /* strcmp */
#include <semaphore.h>/* sem_t */
#include <fcntl.h>    /* O_CREAT */
#include <unistd.h>   /* fflush */

#define PERMISSIONS (0644)

sem_t *sem = NULL;
const char *sem_name = NULL;
ssize_t undo_delta = 0; 
int undo_used = 0;

static void UndoHandler(void) 
{
    int sval = 0;
    ssize_t i = 0;

    if (sem && undo_used)
    {
        sem_getvalue(sem, &sval);

        if (0 < undo_delta) 
        {
            for (i = 0; i < undo_delta; i++)
            {
                sem_wait(sem);
            }
        } 
        else if(0 > undo_delta)
        {
            for (i = 0; i < -undo_delta; i++)
            {
                sem_post(sem);
            }
        }
        printf("UNDO applied: semaphore restored by %ld\n", undo_delta);
    }

    if (sem)
    {
        sem_close(sem);
        sem = NULL;
    }
}

int main(int argc, char *argv[]) 
{
    char cmd = 0;
    int i = 0;
    int sval = 0;
    int args = 0;
    int num = 0;
    int undo = 0;
    char line[256];
    char undo_str[16] = {0};

    if (argc != 2) 
    {
        printf("add a semaphore name!\n");
        return 1;
    }

    sem_name = argv[1];

    atexit(UndoHandler);  

    sem = sem_open(sem_name, O_CREAT, PERMISSIONS, 0);
    if (sem == SEM_FAILED) 
    {
        perror("sem_open");
        return 1;
    }

    
    while (1) 
    {
        printf("semaphore by baruchi :) > ");

        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
        {
            break;
        }

        args = sscanf(line, "%c %d %s", &cmd, &num, undo_str);

        undo = (args == 3 && strcmp(undo_str, "undo") == 0);

        if (cmd == 'X')
        {
            break;
        }
        else if (cmd == 'V')
        {
            if (sem_getvalue(sem, &sval) == -1)
            {
                perror("sem_getvalue");
            }
            else
            {
                printf("Semaphore value: %d\n", sval);
            }
        }
        else if ((cmd == 'D' || cmd == 'I') && args >= 2)
        {
            if (cmd == 'D')
            {
                for (i = 0; i < num; i++)
                {
                    sem_wait(sem);
                }

                if (undo)
                {
                    undo_delta -= num;  
                    undo_used = 1;
                }
            }
            else if (cmd == 'I')
            {
                for (i = 0; i < num; i++)
                {
                    sem_post(sem);
                }

                if (undo)
                {
                    undo_delta += num;  
                    undo_used = 1;
                }
            }

            printf("%c %d executed%s\n", cmd, num, undo ? " with undo" : "");
        }
        else
        {
            printf("Invalid command\n");
        }
    }

    return 0;
}
