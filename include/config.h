#ifndef CONFIG_H
#define CONFIG_H

#include "constants.h"

typedef struct {
    int sample_rate;
    int bit_depth;
    char samples_folder[256];
} Config;

int load_config(const char *filename, Config *config);
void print_config(const Config *config);

#endif