#ifndef __EXC1_H__
#define __EXC1_H__

#include <stddef.h>

/***************************
 Exercise:  WS8
 Date: 	    17/06/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    
***************************/

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


void InitInt(Operator* element, int value);
void InitFloat(Operator* element, float value);
void InitStr(Operator* element, const char* str);


void printAll(Operator* element, size_t size);
void addAll(Operator* element, size_t size);
void clearAll(Operator* element, size_t size);
void freeAll(Operator* element, size_t size);

#endif 

