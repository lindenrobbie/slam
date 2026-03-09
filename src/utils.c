#include <stdio.h>
#include <stdarg.h>
#include "../include/utils.h"

void print_slam(const char *format, ...)
{
    printf("[SLAM]");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}