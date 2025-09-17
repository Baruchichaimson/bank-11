/*****************
 Exercise:	FSQ
 Date:		14/09/25
 Developer: Baruch Haimson
 Reviewer:  
 Status:  Aproved
 *****************/

#ifndef _ILRD_FSQ_H_
#define _ILRD_FSQ_H_

#include <stddef.h> /* size_t */

typedef struct fsq fsq_t;

/* Creates a thread safe FSQ with a maximum capacity */
/* params: capacity - will determine the capacity of the fsq */
/* FSQ will have a minimum capacity of 4 */
/* if allocation failed, NULL will be returned */
fsq_t* FSQCreate(size_t capacity);

/* destroys the FSQ */
/* undefined behaviour will occour if fsq is NULL*/
void FSQDestroy(fsq_t* fsq);

/* pushed an element to the queue */
/* undefined behaviour will occour if fsq is NULL*/
void FSQEnqueue(fsq_t* fsq, void* data);

/* returns the first element (FIFO) */
/* undefined behaviour will occour if fsq is NULL*/
void* FSQDequeue(fsq_t* fsq);

#endif /* _ILRD_FSQ_H_ */
