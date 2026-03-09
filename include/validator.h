#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "config.h"
#include "constants.h"

typedef struct {
    int is_valid;
    char error_msg[MAX_ERROR_MSG_LENGTH];
} ValidationResult;

ValidationResult validate_sample(const char *filename, const Config *config);

#endif