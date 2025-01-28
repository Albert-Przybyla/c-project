#include <stdio.h>
#include <stdlib.h>

int validate_storage(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <conf file path> <string>\n", argv[0]);
        return 0;
    }

    return 1;
}