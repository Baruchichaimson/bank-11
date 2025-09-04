#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD (256)

int main() 
{
    int ret = 0;
    int status = 0;
    pid_t pid = { 0 };
    char cmd[MAX_CMD] = { 0 };
    int use_system = 0;

    printf("Use system() instead of fork? (1=yes, 0=no): ");
    scanf("%d", &use_system);
    getchar(); 

    while (1) 
    {
        printf("simple-shell> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) 
        {
            break; 
        }

        cmd[strcspn(cmd, "\n")] = '\0';

        if (strcmp(cmd, "exit") == 0) 
        {
            break;
        }

        if (use_system) 
        {
            ret = system(cmd);
            if (ret == -1) 
            {
                perror("system");
            }
        } 
        else 
        {
            pid = fork();
            if (pid < 0) 
            {
                perror("fork");
                continue;
            }

            if (pid == 0) 
            { 
                char *argv[] = {"/bin/sh", "-c", cmd, NULL};
                execvp(argv[0], argv);
                perror("execvp");
                _exit(127);
            } 
            else 
            { 
                if (waitpid(pid, &status, 0) < 0) 
                {
                    perror("waitpid");
                }
            }
        }
    }
    printf("Exiting shell.\n");
    return 0;
}
