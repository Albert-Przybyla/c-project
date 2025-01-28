#include <ctype.h>
#include <stdbool.h>

bool is_number(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
            return false;
        str++;
    }
    return true;
}
