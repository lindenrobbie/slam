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

// Process: Check if command contains only allowed characters
// Allowed: letters (a-z, A-Z), spaces, and hyphens
int is_valid_command(const char *command)
{
    
    for (int i = 0; command[i] != '\0'; i++)
    {
        char c = command[i];

        if (!((c >= 'a' && c <= 'z') ||
              (c >= 'A' && c <= 'Z') ||
              c == ' ' ||
              c == '-'))
        {
            return 0;
        }
    }
    return 1;
}