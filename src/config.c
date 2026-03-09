#include <stdio.h>
#include <string.h>
#include "../include/config.h"
#include "../include/constants.h"

void parse_config_line(char *line, Config *config)
{

    char *equals_position = strchr(line, '=');

    if (equals_position == NULL)
    {
        return;
    }

    *equals_position = '\0';
    char *key = line;
    char *value = equals_position + 1;

    if (strcmp(key, "SAMPLE_RATE") == 0)
    {
        sscanf(value, "%d", &config->sample_rate);
    }
    else if (strcmp(key, "BIT_DEPTH") == 0)
    {
        sscanf(value, "%d", &config->bit_depth);
    }

}

int load_config(const char *filename, Config *config)
{
    FILE *config_file = fopen(filename, "r");

    if (config_file == NULL)
    {
        return -1;
    }

    char line[MAX_CONFIG_LINE_LENGTH];

    while (fgets(line, sizeof(line), config_file) != NULL)
    {

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0)
        {
            continue;
        }

        parse_config_line(line, config);

    }

    fclose(config_file);

    return 0;
}

void print_config(const Config *config)
{
    printf("\nConfiguration:\n\n");
    printf("Label: %s\n", config->label);
    printf("Artist: %s\n", config->artist);
    printf("BPM Range: %d - %d\n", config->bpm_min, config->bpm_max);
    printf("Sample Rate: %d Hz\n", config->sample_rate);
    printf("Bit Depth: %d bit\n\n", config->bit_depth);
}