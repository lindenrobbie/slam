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

// Helper function for sort, alphabetically sorts PASS and FAIL in their own list
int compare_results(const void *a, const void *b)
{
    AnalysisResult *result_a = (AnalysisResult *)a;
    AnalysisResult *result_b = (AnalysisResult *)b;

    return strcmp(result_a->filename, result_b->filename);
}

void print_results_table(AnalysisResult *results, int result_count, int passed, int failed)
{
    /*================================*/
    /*       2.1 Sort results         */
    /*================================*/

    // Process 2.1.1: Separate PASS and FAIL results
    AnalysisResult *pass_results = malloc(sizeof(AnalysisResult) * passed);
    AnalysisResult *fail_results = malloc(sizeof(AnalysisResult) * failed);

    int pass_idx = 0;
    int fail_idx = 0;

    for (int i = 0; i < result_count; i++)
    {
        if (results[i].is_valid)
        {
            pass_results[pass_idx++] = results[i];
        }
        else
        {
            fail_results[fail_idx++] = results[i];
        }
    }

    // Process 2.1.2: Sort both lists alphabetically
    qsort(pass_results, passed, sizeof(AnalysisResult), compare_results);
    qsort(fail_results, failed, sizeof(AnalysisResult), compare_results);

    /*================================*/
    /*     2.2 Print table header     */
    /*================================*/

    printf("%-25s %-10s %-15s %-12s %s\n",
           "File", "Status", "Sample Rate", "Bit Depth", "Error");
    printf("--------------------------------------------------------------------------------\n");
        
    /*================================*/
    /*     2.3 Print FAIL results     */
    /*================================*/

    for (int i = 0; i < failed; i++)
    {
        char sample_rate_str[MAX_FORMAT_STRING_LENGTH];
        char bit_depth_str[MAX_FORMAT_STRING_LENGTH];

        // Format sample rate
        if (fail_results[i].sample_rate > 0)
        {
            snprintf(sample_rate_str, sizeof(sample_rate_str), "%d Hz", fail_results[i].sample_rate);
        }
        else
        {
            strcpy(sample_rate_str, "-");
        }

        // Format bit depth
        if (fail_results[i].bit_depth > 0)
        {
            snprintf(bit_depth_str, sizeof(bit_depth_str), "%d bit", fail_results[i].bit_depth);
        }
        else
        {
            strcpy(bit_depth_str, "-");
        }

        printf("%-25s " COLOR_RED "%-10s" COLOR_RESET " %-15s %-12s %s\n",
               fail_results[i].filename,
               "FAIL",
               sample_rate_str,
               bit_depth_str,
               fail_results[i].error_msg);
    }

    /*================================*/
    /*     2.4 Print PASS results     */
    /*================================*/

    
    for (int i = 0; i < passed; i++)
    {
        char sample_rate_str[MAX_FORMAT_STRING_LENGTH];
        char bit_depth_str[MAX_FORMAT_STRING_LENGTH];

        // Format sample rate
        if (pass_results[i].sample_rate > 0)
        {
            snprintf(sample_rate_str, sizeof(sample_rate_str), "%d Hz", pass_results[i].sample_rate);
        }
        else
        {
            strcpy(sample_rate_str, "-");
        }

        // Format bit depth
        if (pass_results[i].bit_depth > 0)
        {
            snprintf(bit_depth_str, sizeof(bit_depth_str), "%d bit", pass_results[i].bit_depth);
        }
        else
        {
            strcpy(bit_depth_str, "-");
        }

        printf("%-25s " COLOR_GREEN "%-10s" COLOR_RESET " %-15s %-12s %s\n",
               pass_results[i].filename,
               "PASS",
               sample_rate_str,
               bit_depth_str,
               "-");
    }
       

    /*================================*/
    /*       2.5 Print summary        */
    /*================================*/
    printf("--------------------------------------------------------------------------------\n");
    printf("Summary: %d passed, %d failed\n", passed, failed);
    printf("--------------------------------------------------------------------------------\n\n");

    /*================================*/
    /*          2.6 Clean up          */
    /*================================*/

    free(pass_results);
    free(fail_results);

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

