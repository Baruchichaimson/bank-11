#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

enum operations { ADD, REMOVE, COUNT , PREFIX, EXIT };

enum operations action_add(const char* filename, const char* input) 
{
    FILE* f = fopen(filename, "a");
    if (!f) 
    {
        perror("Error opening file");
        return ADD;
    }

    fprintf(f, "%s\n", input);
    fclose(f);
    return ADD;
}

enum operations action_remove(const char* filename, const char* input) 
{
    if (remove(filename) == 0)
    {
        printf("File deleted.\n");
    }
    else
    {
        perror("Failed to delete file");
    }
    return REMOVE;
}

enum operations action_count(const char* filename, const char* input) 
{
    int lines = 0;
    char line[SIZE];
    FILE* f = fopen(filename, "r");
    if (!f) 
    {
        perror("Error opening file");
        return COUNT;
    }
    
    while (fgets(line, sizeof(line), f) != NULL) 
    {
        lines++;
    } 
    
    fclose(f);
    printf("Total lines in file: %d\n", lines);
    return COUNT;
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
        return PREFIX;
    }
    
    fprintf(f, "%s\n", input + 1); 
    fprintf(f, "%s", existing_content);
    
    fclose(f);
    printf("Added prefix line: %s\n", input + 1);
    return PREFIX;
}

enum operations action_exit() 
{
    printf("Exiting...\n");
    exit(0);  
    return EXIT;
}

enum operations detect_operation(const char* input)
{
    if (strcmp(input, "-remove") == 0)
        return REMOVE;
    if (strcmp(input, "-count") == 0)
        return COUNT;
    if (strcmp(input, "-exit") == 0)
        return EXIT;
    if (input[0] == '<')
        return PREFIX;
    return ADD;
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

    printf("Logger running. Enter lines (empty to quit):\n");

    while (1) 
    {
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0)
            break;

        enum operations op = detect_operation(buffer);
        enum operations result;

        switch (op)
        {
            case PREFIX:
                result = action_prefix(filename, buffer);
                break;
            case REMOVE:
                result = action_remove(filename, buffer);
                break;
            case COUNT:
                result = action_count(filename, buffer);
                break;
            case EXIT:
                result = action_exit();  
                break;
            case ADD:
                result = action_add(filename, buffer);
                break;
        }
    }

    return 0;
}

