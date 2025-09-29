#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "wd.h"

int main(int argc, char *argv[])
{
    printf("User program started\n");

    if (WDStart(2, 3, (const char **)argv, argc) != SUCCESS)
    {
        printf("Failed to start watchdog\n");
        return 1;
    }

    for (int i = 0; i < 10; ++i)
    {
        printf("User alive... %d\n", i);
        sleep(1);
    }

    WDStop();

    printf("User stopped safely\n");

    return 0;
}
