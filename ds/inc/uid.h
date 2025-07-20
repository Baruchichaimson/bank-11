#ifndef ILRD_UID_H
#define ILRD_UID_H

#include <time.h> /* time_t */
#include <sys/types.h> /* pid_t */

typedef struct uid
{
    size_t counter;
    time_t timestamp;
    pid_t pid;
    unsigned char ip[14];
}ilrd_uid_t;

extern const ilrd_uid_t UIDbadUID;

ilrd_uid_t UIDCreate(void); /* O(1) */
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2); /* O(1) */

#endif /* ILRD_UID_H */





































