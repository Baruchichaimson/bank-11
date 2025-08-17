/************************************
 * Exercise: Recursion 
 * Date: 17/08/25
 * Developer: Baruch Haimson
 * Reviewer: 
 * Status: In Progress
 ************************************/

#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h>  /* size_t */
#include "sll.h"     /* for Node type and SLL functions */

int FibonacciRec(int n);      
int FibonacciIter(int n);    

Node* FlipList(Node* node);  

size_t StrLen(const char* str);             
int StrCmp(const char* s1, const char* s2); 
char* StrCpy(char* dest, const char* src); 
char* StrCat(char* dest, const char* src);  
char* StrStr(const char* haystack, const char* needle); 

#endif /* __RECURSION_H__ */
