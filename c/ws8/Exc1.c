#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Exc1.h"

/***************************
 Exercise:  WS8
 Date: 	    17/06/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    
***************************/


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
	char** p_str = (char**)value;
    char* str = *p_str;
    const char* to_add = " append";
    
    size_t new_len = strlen(str) + strlen(to_add) + 1;
    char* new_str = realloc(str, new_len);
    assert(new_str != NULL);
    
    strcat(new_str, to_add);
    *p_str = new_str;
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
    printf("String: %s\n", *(char**)value); 
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
    (*(char**)value)[0] = '\0';
}


/*****INIT-FUNCTIONS*********/
void InitInt(Operator* element, int value)
{
    element->add = AddInt;
    element->print = PrintInt;
    element->clear = ClearInt;
    *(int*)&element->value = value;
}

void InitFloat(Operator* element, float value)
{
    element->add = AddFloat;
    element->print = PrintFloat;
    element->clear = ClearFloat;
    *(float*)&element->value = value;
}

void InitStr(Operator* element, const char* str)
{
    char* p = (char*)malloc(strlen(str) + 1);
    assert(p != NULL);
    strcpy(p, str);
    
    char** holder = (char**)malloc(sizeof(char*)); 
    assert(holder != NULL);
    *holder = p;

    element->add = AddString;
    element->print = PrintString;
    element->clear = ClearString;
    element->value = holder;
}


/*****PRINTALL & ADDALL & FREEALL FUNCTIONS*********/
void printAll(Operator* element, size_t size)
{
	size_t i;
    for (i = 0; i < size; ++i)
    {
        element[i].print(element[i].value);
    }
}

void addAll(Operator* element, size_t size)
{
	size_t i;
    for (i = 0; i < size; ++i)
    {
        element[i].add(element[i].value);
    }
}

void clearAll(Operator* element, size_t size)
{
	size_t i;
    for (i = 0; i < size; ++i)
    {
        element[i].clear(element[i].value);
    }
}
void freeAll(Operator* element, size_t size)
{
    size_t i;
    for (i = 0; i < size; ++i)
    {
        if (element[i].clear == ClearString)
        {
            free(*(char**)element[i].value);  
            free(element[i].value);           
        }
    }
}

