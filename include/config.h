#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int sample_rate;
    int bit_depth;
} Config;

int load_config(const char *filename, Config *config);
void print_config(const Config *config);

#endif