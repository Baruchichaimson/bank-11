#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

enum operations {CORRECT , ERROR};

struct CommandHandler
{
    char* keyword;
    int (*match)(const char*);
    enum operations (*action)(const char*, const char*);
};

/***************************************
actions functions
***************************************/
enum operations action_add(const char* filename, const char* input) 
{
    FILE* f = fopen(filename, "a");
    if (!f) 
    {
    	perror("Error opening file for add");
    	return ERROR;
    }

    fprintf(f, "%s\n", input);
    fclose(f);
    return CORRECT;
}
enum operations action_remove(const char* filename, const char* input) 
{
    if (remove(filename) == 0)
    {
        printf("File deleted.\n");
        return CORRECT;
    }
    else
    {
        perror("Failed to delete file");
        return ERROR;
    }
}

enum operations action_count(const char* filename, const char* input) 
{
   int lines = 0;
   char line[SIZE];
   FILE* f = fopen(filename, "r");
   if (!f) 
   {
    	perror("Error opening file for count");
    	return ERROR;
   }
   
   while (fgets(line, sizeof(line), f) != NULL) 
   {
       lines++;
   } 
    
   fclose(f);
   printf("Total lines in file: %d\n", lines);
   return CORRECT;
}

enum operations action_prefix(const char* filename, const char* input) 
{
    FILE* f = fopen(filename, "r");
    char existing_content[10000] = "";
    char line[SIZE];
    
    if (f) {
        while (fgets(line, sizeof(line), f) != NULL) {
            strcat(existing_content, line);
        }
        fclose(f);
    }
    
    f = fopen(filename, "w");
    if (!f) {
        perror("Error opening file for writing");
        return ERROR;
    }
    
    fprintf(f, "%s\n", input + 1); 
    
    fprintf(f, "%s", existing_content);
    
    fclose(f);
    printf("Added prefix line: %s\n", input + 1);
    return CORRECT;
}

enum operations action_exit() 
{
    printf("Exiting...\n");
    exit(0);
    return CORRECT;
}

/***************************************
matches functions
***************************************/
int match_remove(const char* input) 
{
    return strcmp(input, "-remove") == 0;
}

int match_count(const char* input) 
{
    return strcmp(input, "-count") == 0;
}

int match_exit(const char* input) 
{
    return strcmp(input, "-exit") == 0;
}

int match_prefix(const char* input) 
{
    return input[0] == '<';
}



int main(int argc, char* argv[]) 
{ 
    if (argc < 2) 
    {
    	fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    	return 1;
    }
    const char* filename = argv[1];
    char buffer[SIZE];
    struct CommandHandler handlers[] = 
    {
	{"-append", match_prefix, action_prefix},
	{"-remove", match_remove, action_remove},
	{"-count", match_count, action_count},
	{"-exit", match_exit, action_exit},
    };
    
    printf("Logger running... Enter string:\n");

    while (1) 
    {
        fgets(buffer, sizeof(buffer), stdin);
	buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0)
        { 
        	break;
        }

        int handled = 0;
        int len = sizeof(handlers) / sizeof(handlers[0]);
        for (int i = 0; i < len; ++i) 
        {
            if(handlers[i].match(buffer)) 
            {
        	enum operations result = handlers[i].action(filename, buffer);
                if (result == ERROR) 
                {
        		fprintf(stderr, "Command failed: %s\n", buffer);
    		}
                handled = 1;
                break;
            }
        }
        if (!handled) 
        {
            action_add(filename, buffer);
        }
    }

    return 0;
}
