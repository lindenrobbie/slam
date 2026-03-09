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

    /*===================*/
    /*   3. Read data    */
    /*===================*/

    DataChunk data_chunk;

    // 3.1 Error: Can't read data chunk header (file corrupted)
    if (fread(&data_chunk, sizeof(DataChunk), 1, wav_file) != 1)
    {
        fclose(wav_file);
        return -1;
    }

    /*===================*/
    /*   4. SUCCESS      */
    /*===================*/

    info->sample_rate = fmt_chunk.sample_rate;
    info->bit_depth = fmt_chunk.bit_depth;
    info->num_channels = fmt_chunk.num_channels;
    info->data_size = data_chunk.data_size;

    fclose(wav_file);

    return 0;
}