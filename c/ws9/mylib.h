#ifndef __MYLIB_H__
#define __MYLIB_H__

#include <stddef.h>

void *MyMemset(void *dest, int c, size_t n);
void *MyMemcpy(void *dest, const void *src, size_t n);
void *MyMemmove(void *dest, const void *src, size_t n);

#endif

