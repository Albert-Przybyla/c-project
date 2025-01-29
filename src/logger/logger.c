#include "logger.h"

void log_message(const char *color, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("%s", color);
    vprintf(format, args);
    printf(RESET);
    va_end(args);
}