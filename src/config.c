#include <stdio.h>
#include <string.h>
#include "../include/config.h"
#include "../include/constants.h"
#include "../include/utils.h"

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

    // Process 1.5: Check for "SAMPLES_FOLDER"
    else if (strcmp(key, "SAMPLES_FOLDER") == 0)
    {
        strcpy(config->samples_folder, value);
    }

}

//===============================================//
//                                               //
//        2. Validate config values              //
//                                               //
//===============================================//

int validate_config(Config *config)
{
    int is_valid = 1;

    /*================================*/
    /*   2.1 Sample rate check        */
    /*================================*/

    // Error 2.1 config.c: Sample rate out of reasonable range
    if (config->sample_rate < MIN_SAMPLE_RATE || config->sample_rate > MAX_SAMPLE_RATE)
    {
        char msg[MAX_ERROR_MSG_LENGTH + MAX_FOLDER_PATH_LENGTH];
        snprintf(msg, sizeof(msg), "Warning 2.1: Invalid sample rate %d Hz, using default %d Hz",
                config->sample_rate, DEFAULT_SAMPLE_RATE);
        print_warning("config.c", msg);
        config->sample_rate = DEFAULT_SAMPLE_RATE;
        is_valid = 0;
    }

    /*================================*/
    /*     2.2 Bit depth check        */
    /*================================*/

    // Error 2.2 config.c: Bit depth not 16, 24, or 32
    if (config->bit_depth != VALID_BIT_DEPTH_1 && config->bit_depth != VALID_BIT_DEPTH_2 && config->bit_depth != VALID_BIT_DEPTH_3)
    {
        char msg[MAX_ERROR_MSG_LENGTH + MAX_FOLDER_PATH_LENGTH];
        snprintf(msg, sizeof(msg), "Warning 2.2: Invalid bit depth %d bit, using default %d bit",
                config->bit_depth, DEFAULT_BIT_DEPTH);
        print_warning("config.c", msg);
        config->bit_depth = DEFAULT_BIT_DEPTH;
        is_valid = 0;
    }

    /*================================*/
    /*   2.3 Folder path check        */
    /*================================*/

    // Error 2.3 config.c: Samples folder path is empty
    if (strlen(config->samples_folder) == 0)
    {
        char msg[MAX_ERROR_MSG_LENGTH + MAX_FOLDER_PATH_LENGTH];
        snprintf(msg, sizeof(msg), "Warning 2.3: Empty samples folder, using default '%s'",
                DEFAULT_SAMPLES_FOLDER);
        print_warning("config.c", msg);
        strcpy(config->samples_folder, DEFAULT_SAMPLES_FOLDER);
        is_valid = 0;
    }

    return is_valid;
}

//===============================================//
//                                               //
//        3. Load config from file               //
//                                               //
//===============================================//

int load_config(const char *filename, Config *config)
{
    /*================================*/
    /*      3.1 Open config file      */
    /*================================*/

    FILE *config_file = fopen(filename, "r");

    // Error 3.1 config.c: File doesn't exist or can't be opened
    if (config_file == NULL)
    {
        return -1;
    }


    /*================================*/
    /*  3.2 Read and parse each line  */
    /*================================*/

    char line[MAX_CONFIG_LINE_LENGTH];

    // Process 3.2.1: Read file line by line
    while (fgets(line, sizeof(line), config_file) != NULL)
    {
        // Process 3.2.2: Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Process 3.2.3: Skip empty lines
        if (strlen(line) == 0)
        {
            continue;
        }

        // Process 3.2.4: Parse the line
        parse_config_line(line, config);

    }

    fclose(config_file);

    // Process 3.2.4: Validate loaded config values
    validate_config(config);

    return 0;
}

//===============================================//
//      4. Print format for config print         //
//===============================================//

void print_config(const Config *config)
{
    printf("\nConfiguration:\n\n");
    printf("Sample Rate: %d Hz\n", config->sample_rate);
    printf("Bit Depth: %d bit\n", config->bit_depth);
    printf("Folder: %s\n\n", config->samples_folder);
}