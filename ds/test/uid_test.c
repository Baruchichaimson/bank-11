#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "uid.h"

int main()
{
	size_t i = 0;
	ilrd_uid_t uid1;
	ilrd_uid_t uid2;
	ilrd_uid_t uid[8];
	for(i = 0; i < 8; i++)
	{
		sleep(1);
		uid[i] = UIDCreate();
		printf("%lu\n", uid[i].counter);
		printf("%lu\n", uid[i].timestamp);
		printf("%d\n", uid[i].pid);
		printf("%s\n", uid[i].ip);
	}

	uid1 = UIDCreate();
	sleep(1);
	uid2 = UIDCreate();
	if(!UIDIsSame(uid1, uid2))
	{
		printf("success\n");
	}

	return 0;
}
