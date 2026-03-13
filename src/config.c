#include <stdio.h>
#include <string.h>
#include "../include/config.h"
#include "../include/constants.h"

//===============================================//
//                                               //
//        1. Parse single config line            //
//                                               //
//===============================================//

void parse_config_line(char *line, Config *config)
{
    // Process 1.1 Split line by "="
    char *equals_position = strchr(line, '=');

    // Error 1.1 config.c: No '=' found in line (invalid format)
    if (equals_position == NULL)
    {
        return;
    }

    // Process 1.2: Split into key and value
    *equals_position = '\0';
    char *key = line;
    char *value = equals_position + 1;

    // Process 1.3: Check for "SAMPLE_RATE"
    if (strcmp(key, "SAMPLE_RATE") == 0)
    {
        sscanf(value, "%d", &config->sample_rate);
    }
    // Process 1.4: Check for "BIT_DEPTH"
    else if (strcmp(key, "BIT_DEPTH") == 0)
    {
        sscanf(value, "%d", &config->bit_depth);
    }

}

//===============================================//
//                                               //
//        2. Load config from file               //
//                                               //
//===============================================//

int load_config(const char *filename, Config *config)
{
    /*================================*/
    /*      2.1 Open config file      */
    /*================================*/

    FILE *config_file = fopen(filename, "r");

    // Error 2.1 config.c: File doesn't exist or can't be opened
    if (config_file == NULL)
    {
        return -1;
    }


    /*================================*/
    /*  2.2 Read and parse each line  */
    /*================================*/

    char line[MAX_CONFIG_LINE_LENGTH];

    // Process 2.2.1: Read file line by line
    while (fgets(line, sizeof(line), config_file) != NULL)
    {
        // Process 2.2.2: Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Process 2.2.3: Skip empty lines
        if (strlen(line) == 0)
        {
            continue;
        }

        // Process 2.2.4: Parse the line
        parse_config_line(line, config);

    }

    fclose(config_file);
    return 0;
}

//===============================================//
//      3. Print format for config print         //
//===============================================//

void print_config(const Config *config)
{
    printf("\nConfiguration:\n\n");
    printf("Sample Rate: %d Hz\n", config->sample_rate);
    printf("Bit Depth: %d bit\n\n", config->bit_depth);
}