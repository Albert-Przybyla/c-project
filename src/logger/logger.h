#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"

void log_message(const char *color, const char *format, ...);

#define init(...) log_message(BLUE, __VA_ARGS__)
#define info(...) log_message(GREEN, __VA_ARGS__)
#define warn(...) log_message(YELLOW, __VA_ARGS__)
#define error(...) log_message(RED, __VA_ARGS__)

#endif