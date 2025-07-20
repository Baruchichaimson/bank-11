#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "uid.h"

const ilrd_uid_t UIDbadUID = {0, 0, 0, {0}};
static size_t global_counter = 0;

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t uid;
		

	uid.counter += global_counter;
    uid.timestamp = time(NULL);
    uid.pid = getpid();
    getip(uid.ip) ? uid.ip : UIDbadUID;
}
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return (uid1.pid == uid2.pid &&
	 		uid1.counter == uid2.counter &&
	  		uid1.timestamp == uid2.timestamp &&
	   		memcmp(uid1.ip, uid2.ip, 14) == 0)
}

/***************** help function ************************************/

static void getip(unsigned char *ip_out)
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;
    int found = 0;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        memset(ip_out, 0, 14); 
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) 
        {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;

            if (ntohl(sa->sin_addr.s_addr) != INADDR_LOOPBACK)
            {
                memcpy(ip_out, &sa->sin_addr, 4);
                memset(ip_out + 4, 0, 10); 
                found = 1;
                break;
            }
        }
    }

    if (!found)
    {
        memset(ip_out, 0, 14);
    }

    freeifaddrs(ifaddr);
}

