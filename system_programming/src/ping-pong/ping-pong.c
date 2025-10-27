#define _POSIX_C_SOURCE 200809L

#include <stdio.h>    /* printf, perror */
#include <stdlib.h>   /* exit, EXIT_FAILURE */
#include <unistd.h>   /* fork, getpid, pause, sleep */
#include <signal.h>   /* sigaction, kill, SIGUSR1, SIGUSR2 */

volatile sig_atomic_t got_signal = 0;

static void handler(int sig) 
{
    got_signal = sig;
}

int main() 
{
    int time = 15;
    pid_t child_pid;
    pid_t parent_pid;
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) 
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1) 
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    parent_pid = getpid();

    child_pid = fork();
    if (child_pid < 0) 
    {
        perror("fork failed");
        exit(1);
    }

    if (child_pid == 0) 
    {
        while (time) 
        {
            pause(); 
            if (got_signal == SIGUSR1) 
            {
                printf("\n-> child received SIGUSR1, sending SIGUSR2 to parent ->\n");
                kill(parent_pid, SIGUSR2);
                got_signal = 0;
            }
            time--;
        }
    } 
    else 
    {
        sleep(1); 
        printf("Parent sending SIGUSR1 to child\n");
        if (kill(child_pid, SIGUSR1) == -1) 
        {
            perror("kill");
            exit(EXIT_FAILURE);
        }

        while (time) 
        {
            pause();
            if (got_signal == SIGUSR2) 
            {
                printf("\n<- parent received SIGUSR2, sending SIGUSR1 to child <-\n");
                kill(child_pid, SIGUSR1);
                got_signal = 0;
            }
            time--;
        }
    }
    return 0;
}
