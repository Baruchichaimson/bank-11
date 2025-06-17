#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*typedef enum 
{
    TYPE_INT,
    TYPE_STRING,
    TYPE_FLOAT
} DataType;*/

typedef void (*AddFunc)(void* value);
typedef void (*PrintFunc)(void* value);
typedef void (*ClearFunc)(void* value);

typedef struct 
{
    AddFunc add;
    PrintFunc print;
    ClearFunc clear;
    void* value;
} Operator;


/*****ADD-FUNCTIONS*********/
void AddInt(void* value)
{ 
	(*(int*)value) += 10; 
}
void AddFloat(void* value)
{ 
	(*(float*)value) += 1.5f; 
}
void AddString(void* value)   
{ 
	strcat((char*)value, " append"); 
}


/*****PRINT-FUNCTIONS*********/
void PrintInt(void* value)    
{ 
	printf("Int: %d\n", *(int*)value); 
}
void PrintFloat(void* value)  
{ 
	printf("Float: %.2f\n", *(float*)value); 
}
void PrintString(void* value) 
{ 
	printf("String: %s\n", (char*)value); 
}


/*****CLEAR-FUNCTIONS*********/
void ClearInt(void* value)    
{ 
	*(int*)value = 0; 
}
void ClearFloat(void* value)  
{ 
	*(float*)value = 0.0f; 
}
void ClearString(void* value) 
{ 
	((char*)value)[0] = '\0';
}

int main()
{
	size_t i;
	size_t SizeOps;
    int i1 = 7, i2 = 15;
    float f1 = 3.14f, f2 = 2.71f;
    char* c1;
    char* c2;
    
    c1 = (char*)malloc(100 * sizeof(char));
    assert(c1 != NULL);
    strcpy(c1, "haimson");

    c2 = (char*)malloc(100 * sizeof(char));
    assert(c2 != NULL);
    strcpy(c2, "baruchi");


    Operator ops[] = {
        { AddInt,    PrintInt,    ClearInt,    &i1 },
        { AddInt,    PrintInt,    ClearInt,    &i2 },
        { AddFloat,  PrintFloat,  ClearFloat,  &f1 },
        { AddFloat,  PrintFloat,  ClearFloat,  &f2 },
        { AddString, PrintString, ClearString, c1 },
        { AddString, PrintString, ClearString, c2 }
    };

    SizeOps = sizeof(ops) / sizeof(ops[0]);

    printf("=== Before ADD ===\n");
    
    for(i = 0; i < SizeOps; ++i)
        ops[i].print(ops[i].value);

    printf("\n=== After Add ===\n");
    
    for(i = 0; i < SizeOps; ++i)
        ops[i].add(ops[i].value);
    for(size_t i = 0; i < SizeOps; ++i)
        ops[i].print(ops[i].value);

    printf("\n=== After Clear ===\n");
    
    for(i = 0; i < SizeOps; ++i)
        ops[i].clear(ops[i].value);
    for(i = 0; i < SizeOps; ++i)
        ops[i].print(ops[i].value);
        
    free(c1);
    free(c2);

    return 0;
}

