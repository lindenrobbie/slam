#include <stdio.h>
#include <string.h>
#include "../include/validator.h"
#include "../include/wav_reader.h"

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