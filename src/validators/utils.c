#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_number(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int get_key(char *argv)
{
    int res = 0;
    for (int i = 0; i < strlen(argv); i++)
    {
        res += (int)argv[i];
    }
    return res;
}