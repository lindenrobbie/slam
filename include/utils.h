#ifndef UTILS_H
#define UTILS_H

void print_prompt(void);
void print_error(const char *file, const char *message);
void print_warning(const char *file, const char *message);
int is_valid_command(const char *command);

#endif