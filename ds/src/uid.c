/***************************
 * Exercise: uid
 * Date:     20/07/25
 * Developer: Baruch Haimson
 * Reviewer: matan
 * Status:   Aproved
 ***************************/


#include <ifaddrs.h>    /* getifaddrs, freeifaddrs, struct ifaddrs */
#include <stdio.h>      /* printf, perror */
#include <stdlib.h>     /* malloc, free, exit */
#include <unistd.h>     /* getpid, sleep */
#include <string.h>     /* memcpy, memset, strlen */
#include <pthread.h>    /* pthread_mutex_lock */
#include <sys/socket.h> /* struct sockaddr, socket functions */
#include <netinet/in.h> /* struct sockaddr_in, htons */
#include <arpa/inet.h>  /* inet_ntop, inet_pton */
#include <time.h> 

#include "uid.h"        /* ilrd_uid_t, UIDCreate */

static char* getip(char *ip_out);
static pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

const ilrd_uid_t UIDbadUID = {0, 0, 0, {0}};
static size_t global_counter = 0;

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t uid;
	char* ip;

	pthread_mutex_lock(&counter_lock);
    uid.counter = global_counter++;
    pthread_mutex_unlock(&counter_lock);

    uid.timestamp = time(NULL);
    uid.pid = getpid();
	ip = getip((char *)uid.ip);
    if (NULL == ip)
    {
        return UIDbadUID;
    }
    memcpy(uid.ip , ip, strlen(ip));
    
	return uid;
}
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return (uid1.pid == uid2.pid &&
	 		uid1.counter == uid2.counter &&
	  		uid1.timestamp == uid2.timestamp &&
	   		memcmp(uid1.ip, uid2.ip, 14) == 0);
}

/***************** help function ************************************/

static char* getip(char *ip_out)
{
    struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	struct sockaddr_in *sa;
	const char *res;
    int found = 0;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        return NULL;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL) continue;

        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            sa = (struct sockaddr_in *)ifa->ifa_addr;

            if (ntohl(sa->sin_addr.s_addr) != INADDR_LOOPBACK)
            {
                res = inet_ntop(AF_INET, &(sa->sin_addr), ip_out, 14);
                if (res != NULL)
                {
                    found = 1;
                    break;
                }
            }
        }
    }

    freeifaddrs(ifaddr);
    return found ? ip_out : NULL;
}








