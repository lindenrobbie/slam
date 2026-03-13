#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/validator.h"
#include "../include/wav_reader.h"

// This is to compile for windows, I am using Linux in WSL
#ifdef _WIN32
    #include "../include/dirent_windows.h"
#else
    #include <dirent.h>
#endif

//===============================================//
//                                               //
//        1.  Single file validator              //
//                                               //
//===============================================//

ValidationResult validate_sample(const char *filename, const Config *config)
{

    ValidationResult result;
    result.is_valid = 1;
    strcpy(result.error_msg, "");

    /*====================*/
    /*   1.1. Open file   */
    /*====================*/
    WavInfo wav_info;
    int read_status = read_wav_info(filename, &wav_info);

    // Error 1.1 validator.c: File doesn't exist or can't be opened
    if (read_status != 0)
    {
        result.is_valid = 0;
        strcpy(result.error_msg, "Could not read WAV file");
        return result;
    }

    /*==============================*/
    /*     1.2. Validate specs      */
    /*==============================*/

    // Process 1.2.1: Sample rate check
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

    // Process 1.2.2: Bit depth check
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

    // Process 1.2.3: Both match
    return result;
}

//===============================================//
//                                               //
//         2. Result table print format          //
//                                               //
//===============================================//

void print_results_table(AnalysisResult *results, int result_count, int passed, int failed)
{
    // Table header
    printf("%-25s %-10s %-15s %-12s %s\n",
           "File", "Status", "Sample Rate", "Bit Depth", "Error");
    printf("--------------------------------------------------------------------------------\n");

    
    for (int i = 0; i < result_count; i++)
    {
        char sample_rate_str[MAX_FORMAT_STRING_LENGTH];
        char bit_depth_str[MAX_FORMAT_STRING_LENGTH];

        /*=========================================*/
        /*    2.1. Format data nicely preprint     */
        /*=========================================*/

        // Process 2.1.1: Format it as e.g "44100 Hz"
        if (results[i].sample_rate > 0)
        {
            snprintf(sample_rate_str, sizeof(sample_rate_str), "%d Hz", results[i].sample_rate);
        }
        // Otherwise, format as "-" (e.g when file could not be read)
        else
        {
            strcpy(sample_rate_str, "-");
        }
        // Process 2.1.2: Format bit depth (same as sample rate)
        if (results[i].bit_depth > 0)
        {
            snprintf(bit_depth_str, sizeof(bit_depth_str), "%d bit", results[i].bit_depth);
        }
        // Otherwise, format as "-" (e.g when file could not be)
        else
        {
            strcpy(bit_depth_str, "-");
        }

        /*================================*/
        /*    2.2. Print data finally     */
        /*================================*/

        // Process 2.2.1: Print PASS samples
        if (results[i].is_valid)
        {
            printf("%-25s " COLOR_GREEN "%-10s" COLOR_RESET " %-15s %-12s %s\n",
                   results[i].filename,
                   "PASS",
                   sample_rate_str,
                   bit_depth_str,
                   "-");
        }
        // Process 2.2.2: Print FAIL samples
        else
        {
            printf("%-25s " COLOR_RED "%-10s" COLOR_RESET " %-15s %-12s %s\n",
                   results[i].filename,
                   "FAIL",
                   sample_rate_str,
                   bit_depth_str,
                   results[i].error_msg);
        }
    }

    // Process 2.2.3: Print table tail (Summary)
    printf("--------------------------------------------------------------------------------\n");
    printf("Summary: %d passed, %d failed\n", passed, failed);
    printf("--------------------------------------------------------------------------------\n\n");

}

//===============================================//
//                                               //
//           3. Validate all samples             //
//                                               //
//===============================================//

void analyze_samples_folder(const Config *config)
{
    printf("\nAnalyzing samples in './samples' folder...\n\n");

    /*======================*/
    /* 3.1. Open DIRECTORY  */
    /*======================*/

    DIR *dir = opendir("samples");

    // Error 3.1 validator.c: Folder doesn't exist or can't be opened
    if (dir == NULL)
    {
        printf("Error: Could not open samples folder\n\n");
        return;
    }

    // Process 3.1.1: Count how many files we have (for memory allocation)
    int file_count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        file_count++;
    }
    closedir(dir);

    /*==================================*/
    /* 3.2. Dynamically allocate memory */
    /*==================================*/

    AnalysisResult *results = malloc(sizeof(AnalysisResult) * file_count);

    // Error 3.2 validator.c: Memory allocation failure
    if (results == NULL)
    {
        printf("Error: Memory allocation failed\n\n");
        return;
    }

    /*================================*/
    /*    3.3 Validate each file      */
    /*================================*/

    dir = opendir("samples");
    int result_count = 0;
    int passed = 0;
    int failed = 0;
    char filepath[MAX_PATH_LENGTH];

    while ((entry = readdir(dir)) != NULL)
    {
        // Process 3.3.1: Skip . and .. 
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Process 3.3.2: Build file path
        snprintf(filepath, sizeof(filepath), "samples/%s", entry->d_name);

        // Process 3.3.3: Read WAV file info
        WavInfo wav_info;
        int read_status = read_wav_info(filepath, &wav_info);

        // Process 3.3.4: Store file name and check if it is readable
        strcpy(results[result_count].filename, entry->d_name);

        // Process 3.3.5: Store corrupted file as error
        if (read_status != 0)
        {
            results[result_count].is_valid = 0;
            results[result_count].sample_rate = 0;
            results[result_count].bit_depth = 0;
            strcpy(results[result_count].error_msg, "Could not read WAV file");
            failed++;
        }

        // Process 3.3.6: Validate the sample vs config
        else
        {
            ValidationResult result = validate_sample(filepath, config);
            results[result_count].is_valid = result.is_valid;
            results[result_count].sample_rate = wav_info.sample_rate;
            results[result_count].bit_depth = wav_info.bit_depth;
            strcpy(results[result_count].error_msg, result.error_msg);

            // Process 3.3.7: Count pass/fail for summary
            if (result.is_valid)
            {
                passed++;
            }
            else
            {
                failed++;
            }
        }
        result_count++;
    }

    closedir(dir);

    /*================================*/
    /*        3.4 Print results       */
    /*================================*/

    print_results_table(results, result_count, passed, failed);

    /*================================*/
    /*        3.5 Clean up            */
    /*================================*/
    free(results);
}

