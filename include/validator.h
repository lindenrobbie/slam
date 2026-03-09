#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "config.h"
#include "constants.h"

typedef struct {
    int is_valid;
    char error_msg[MAX_ERROR_MSG_LENGTH];
} ValidationResult;

ValidationResult validate_sample(const char *filename, const Config *config);
void analyze_samples_folder(const Config *config);

#endif