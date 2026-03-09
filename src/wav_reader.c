// Here I learned that ANY binary can be decompiled if you know the structure of the files

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/wav_reader.h"
#include "../include/constants.h"

//Wave file consists of 3 parts:

// 1. WAV file header
typedef struct {
    char riff[4];
    uint32_t file_size;
    char wave[4];
} RiffHeader;

// 2. Metadata
typedef struct {
    char fmt[4];
    uint32_t chunk_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bit_depth;
} FmtChunk;

// 3. Data itself (audio version of a packet's "payload")
typedef struct {
    char data[4];
    uint32_t data_size;
} DataChunk;



int read_wav_info(const char *filename, WavInfo *info)
{   
    /*===================*/
    /*  1. Read header   */
    /*===================*/

    FILE *wav_file = fopen(filename, "rb");

    // 1.1 Error: File doesn't exist or can't be opened
    if (wav_file == NULL)
    {
        return -1;
    }

    RiffHeader riff_header;

    // 1.2 Error: File too short or corrupted (can't read header)
    if (fread(&riff_header, sizeof(RiffHeader), 1, wav_file) != 1)
    {
        fclose(wav_file);
        return -1;
    }
    // 1.3 Error:  Not a valid WAV file (missing "RIFF" or "WAVE")
    if (strncmp(riff_header.riff, "RIFF", 4) != 0 || strncmp(riff_header.wave, "WAVE", 4) != 0)
    {
        fclose(wav_file);
        return -1;
    }

    /*========================================*/
    /*  Step 2: Search for fmt chunk         */
    /*========================================*/

    char chunk_id[4];
    uint32_t chunk_size;
    int fmt_found = false;
    FmtChunk fmt_chunk;

    // WAV files can have chunks in any order, so we search for exactly "fmt "
    while (fmt_found == false)
    {
        // 2.1 Error: Can't read chunk ID (file corrupted)
        if (fread(chunk_id, 4, 1, wav_file) != 1)
        {
            fclose(wav_file);
            return -1;
        }

        // 2.2 Error: Can't read chunk size (file corrupted)
        if (fread(&chunk_size, 4, 1, wav_file) != 1)
        {
            fclose(wav_file);
            return -1;
        }

        // Checks if this is the format chunk
        if (strncmp(chunk_id, "fmt ", 4) == 0)
        {
            // Found it! Read the rest of the fmt data
            fmt_found = true;

            // 2.3 Error: Can't read format data (file corrupted)
            if (fread(&fmt_chunk.audio_format, FMT_CHUNK_DATA_SIZE, 1, wav_file) != 1)
            {
                fclose(wav_file);
                return -1;
            }

            // Some fmt chunks have extra data, skips it
            if (chunk_size > FMT_CHUNK_DATA_SIZE)
            {
                fseek(wav_file, chunk_size - FMT_CHUNK_DATA_SIZE, SEEK_CUR);
            }
        }
        else
        {
            // This isn't the fmt chunk, skip over it
            fseek(wav_file, chunk_size, SEEK_CUR);
        }
    }

    /*========================================*/
    /*  Step 3: Search for data chunk        */
    /*========================================*/

    int data_found = false;

    // Search for the "data" chunk
    while (data_found == false)
    {
        // 3.1 Error: Can't read chunk ID (file corrupted)
        if (fread(chunk_id, 4, 1, wav_file) != 1)
        {
            fclose(wav_file);
            return -1;
        }

        // 3.2 Error: Can't read chunk size (file corrupted)
        if (fread(&chunk_size, 4, 1, wav_file) != 1)
        {
            fclose(wav_file);
            return -1;
        }

        // Check if this is the data chunk
        if (strncmp(chunk_id, "data", 4) == 0)
        {
            // Found it! Extract the info we need
            data_found = true;
            info->sample_rate = fmt_chunk.sample_rate;
            info->bit_depth = fmt_chunk.bit_depth;
            info->num_channels = fmt_chunk.num_channels;
            info->data_size = chunk_size;
        }
        else
        {
            // Not the data chunk, skip it
            fseek(wav_file, chunk_size, SEEK_CUR);
        }
    }

    fclose(wav_file);
    return 0;

}