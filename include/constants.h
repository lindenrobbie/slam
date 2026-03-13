#ifndef CONSTANTS_H
#define CONSTANTS_H

//General info:
#define SLAM_VERSION "1.0"
#define SLAM_AUTHOR "Robbie Linden"
#define SLAM_YEAR "2026"

//Input limits:
#define MAX_COMMAND_LENGTH 50
#define MAX_PATH_LENGTH 512
#define MAX_FOLDER_PATH_LENGTH 256
#define MAX_CONFIG_LINE_LENGTH 256
#define MAX_ERROR_MSG_LENGTH 256
#define MAX_FORMAT_STRING_LENGTH 20

//Colors to make CLI less insufferable to read:
#define COLOR_RESET   "\033[0m"
#define COLOR_ORANGE  "\033[38;5;208m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"

//Default config values:
#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_BIT_DEPTH 24
#define DEFAULT_SAMPLES_FOLDER "samples"

// WAV file format constants:
#define FMT_CHUNK_DATA_SIZE 16 // Size of standard PCM format chunk data (after chunk ID and size)
#define MIN_SAMPLE_RATE 8000      // Minimum reasonable sample rate (8 kHz)
#define MAX_SAMPLE_RATE 192000    // Maximum reasonable sample rate (192 kHz)
#define VALID_BIT_DEPTH_1 16      // Standard bit depth option
#define VALID_BIT_DEPTH_2 24      // Standard bit depth option
#define VALID_BIT_DEPTH_3 32      // Standard bit depth option

#endif