#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char label[32];
    char artist[32];
    int bpm_min;
    int bpm_max;
    int sample_rate;
    int bit_depth;
} Config;

int load_config(const char *filename, Config *config);
void print_config(const Config *config);

#endif