#include <stdio.h>
#include <string.h>
#include "../include/validator.h"
#include "../include/wav_reader.h"

#ifdef _WIN32
    #include "../include/dirent_windows.h"
#else
    #include <dirent.h>
#endif

ValidationResult validate_sample(const char *filename, const Config *config)
{
    ValidationResult result;
    result.is_valid = 1;
    strcpy(result.error_msg, "");

    // 1. Try to read the WAV file
    WavInfo wav_info;
    int read_status = read_wav_info(filename, &wav_info);

    if (read_status != 0)
    {
        result.is_valid = 0;
        strcpy(result.error_msg, "Could not read WAV file");
        return result;
    }

    // 2. Check if sample rate matches config
    int expected_sample_rate = config->sample_rate;
    int actual_sample_rate = wav_info.sample_rate;

    if (actual_sample_rate != expected_sample_rate)
    {
        result.is_valid = 0;
        snprintf(result.error_msg, sizeof(result.error_msg),
                 "Wrong sample rate: %d Hz (expected %d Hz)",
                 actual_sample_rate, expected_sample_rate);
        return result;
    }

    // 3. Check if bit depth matches config
    int expected_bit_depth = config->bit_depth;
    int actual_bit_depth = wav_info.bit_depth;

    if (actual_bit_depth != expected_bit_depth)
    {
        result.is_valid = 0;
        snprintf(result.error_msg, sizeof(result.error_msg),
                 "Wrong bit depth: %d bit (expected %d bit)",
                 actual_bit_depth, expected_bit_depth);
        return result;
    }

    // 4. success woohoo
    return result;
}

void analyze_samples_folder(const Config *config)
{
    printf("\nAnalyzing samples in './samples' folder...\n\n");

    DIR *dir = opendir("samples");
    if (dir == NULL)
    {
        printf("Error: Could not open samples folder\n\n");
        return;
    }

    int passed = 0;
    int failed = 0;
    struct dirent *entry;
    char filepath[MAX_PATH_LENGTH];

    while ((entry = readdir(dir)) != NULL)
    {
    // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Build full path
        snprintf(filepath, sizeof(filepath), "samples/%s", entry->d_name);

        // Validate the sample
        ValidationResult result = validate_sample(filepath, config);

        if (result.is_valid)
        {
        printf(COLOR_GREEN "[PASS]" COLOR_RESET " %s\n", entry->d_name);
        passed++;
        }

        else
        {
        printf(COLOR_RED "[FAIL]" COLOR_RESET " %s\n", entry->d_name);
        printf("       Error: %s\n", result.error_msg);
        failed++;
        }

    }

    closedir(dir);

    printf("\n----------------------------------------\n");
    printf("Summary: %d passed, %d failed\n", passed, failed);
    printf("----------------------------------------\n\n");
}