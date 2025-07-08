/***************************
 Exercise:  cbuff
 Date: 	    07/07/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:    Approved
***************************/

#ifndef _ILRD_CBUFF_
#define _ILRD_CBUFF_

#define DEFAULT_SIZE (8)

#include <stddef.h>
#include <sys/types.h>

typedef struct cbuff cbuff_t;

/* Complexity O(1) */
cbuff_t* CBuffCreate(size_t capacity);

/* Complexity O(1) */
void CBuffDestroy(cbuff_t* cbuff);

/* Complexity O(1) , Return num of bytes succeded to write (num bytes of free space) / -1(error) if failed to write (0 free space to write) */
ssize_t CBuffWrite(cbuff_t* cbuff, const void* src, size_t bytes);

/* Complexity O(1) , Return num of bytes succeded to read (num bytes to read) / -1(error) if nfailed to read (0 bytes to read) */
ssize_t CBuffRead(cbuff_t* cbuff, void* dst, size_t bytes);

/* Complexity O(1) */
int CBuffIsEmpty(cbuff_t* cbuff);

/* Complexity O(1) */
size_t CBuffFreeSpace(cbuff_t* cbuff);

/* Complexity O(1) */
size_t CBuffSize(cbuff_t* cbuff);

#endif /* _ILRD_CBUFF_ */

