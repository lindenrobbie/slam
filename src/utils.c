#include <stdio.h>
#include "../include/utils.h"
#include "../include/constants.h"

void print_prompt(void)
{
    printf("[" COLOR_ORANGE "SLAM" COLOR_RESET "]> ");
}

void print_error(const char *file, const char *message)
{
    fprintf(stderr, COLOR_RED "[ERROR] " COLOR_RESET "%s: %s\n", file, message);
    printf("\n");
}

void print_warning(const char *file, const char *message)
{
    fprintf(stderr, COLOR_YELLOW "[WARNING] " COLOR_RESET "%s: %s\n", file, message);
    printf("\n");
}