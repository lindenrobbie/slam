// Here I learned that ANY binary can be decompiled if you know the structure of the files

#include <stdio.h>
#include <string.h>
#include "../include/wav_reader.h"

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

    /*===================*/
    /*  2. Read metadata */
    /*===================*/

    FmtChunk fmt_chunk;

    // 2.1 Error: Can't read format chunk (file corrupted)
    if (fread(&fmt_chunk, sizeof(FmtChunk), 1, wav_file) != 1)
    {
        fclose(wav_file);
        return -1;
    }

    // 2.2 Error:  Format chunk is invalid (missing "fmt ")
    if (strncmp(fmt_chunk.fmt, "fmt ", 4) != 0)
    {
        fclose(wav_file);
        return -1;
    }

    // 2.3 Skip extra bytes in fmt chunk if it's bigger than 16
    if (fmt_chunk.chunk_size > 16)
    {
        fseek(wav_file, fmt_chunk.chunk_size - 16, SEEK_CUR);
    }

    /*===================*/
    /*   3. Read data    */
    /*===================*/

   char chunk_id[4];
    uint32_t chunk_size;
    int data_found = 0;

    // Keep reading chunks until we find "data"
    while (data_found == 0)
    {
        // Read chunk ID
        if (fread(chunk_id, 4, 1, wav_file) != 1)
        {
            fclose(wav_file);
            return -1;
        }

        // Read chunk size
        if (fread(&chunk_size, 4, 1, wav_file) != 1)
        {
            fclose(wav_file);
            return -1;
        }

        // Check if this is the data chunk
        if (strncmp(chunk_id, "data", 4) == 0)
        {
            // Found it!
            data_found = 1;
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