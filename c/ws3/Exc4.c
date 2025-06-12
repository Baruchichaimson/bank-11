#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/***************************
 Exercise:  WS3
 Date: 	    05/08/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    Approved
***************************/
char **CopyLowerEnv(char **envp) 
{
    
    size_t len;
    size_t i;
    size_t count = 0;
    char *src = NULL;
    char **copy = NULL;
    char **dest = NULL;
    char **p = envp;
    while (*p++)
    { 
        count++;
    }

    copy = malloc((count + 1) * sizeof(char *));
    if (!copy) 
    {
        return NULL;
    }

    dest = copy;
    p = envp;
    while (*p) 
    {
        src = *p;
	len = strlen(src);
	*dest = malloc((len + 1) * sizeof(char *));
	if (!*dest)
	{ 
		return NULL;
	}

	for (i = 0; i < len; ++i)
	{
	    (*dest)[i] = tolower((unsigned char)src[i]);
	}
	(*dest)[len] = '\0';
	
        dest++;
        p++;
    }
    
    *dest = NULL;
    return copy;

}

void PrintEnv(char **env) 
{
    for (char **p = env; *p; p++) 
    {
        printf("%s\n", *p);
    }
}

void FreeEnv(char **env) 
{
    char **p = env;
    while (*p) 
    {
        free(*p);
        p++;
    }
    free(env);
}

int main(int argc, char **argv, char **envp) 
{
    (void)argc; 
    (void)argv;
    char **env_copy = CopyLowerEnv(envp);
    if (!env_copy) 
    {
    	printf("Memory allocation failed\n");
        return 1;
    }
    PrintEnv(env_copy);
    FreeEnv(env_copy);

    return 0;
}

