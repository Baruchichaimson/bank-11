#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

size_t StrLen(const char* str);
int StrCmp(const char *s1,const char *s2);
char* StpCpy(char *dst, const char *src);
char* StrnCpy(char *dst, const char*src, size_t n);
int StrnCmp(const char* s1, const char* s2, size_t n); 
int StrCaseCmp(const char *s1, const char *s2);
char* StrChr(const char *str, int c);  
char* StrDup(const char *src);
char* StrCat(char *dest, const char *src);
char* StrnCat(char *dest, const char *src, size_t n);
char* StrStr(const char *haystack, const char *needle);
size_t StrSpn(const char *s, const char *accept);
char* StrTok(char *str, const char *delim);

#endif

