// Here I learned that ANY binary can be decompiled if you know the structure of the files

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/wav_reader.h"
#include "../include/constants.h"
#include "../include/utils.h"

int read_wav_info(const char *filename, WavInfo *info)
{   
    /*===================*/
    /*   1. Open file    */
    /*===================*/

    FILE *wav_file = fopen(filename, "rb");

    // Error 1.1 wav_reader.c: File doesn't exist or can't be opened
    if (wav_file == NULL)
    {
        print_error("wav_reader.c", "Error 1.1: File doesn't exist or can't be opened");
        return -1;
    }

    /*===================*/
    /*  2. Read header   */
    /*===================*/

    RiffHeader riff_header;

    // Error 2.1 wav_reader.c: File too short or corrupted (can't read header)
    if (fread(&riff_header, sizeof(RiffHeader), 1, wav_file) != 1)
    {
        print_error("wav_reader.c", "Error 2.1: File too short or corrupted (can't read header)");
        fclose(wav_file);
        return -1;
    }
    // Error 2.2 wav_reader.c:  Not a valid WAV file (missing "RIFF" or "WAVE")
    if (strncmp(riff_header.riff, "RIFF", 4) != 0 || strncmp(riff_header.wave, "WAVE", 4) != 0)
    {
        print_error("wav_reader.c", "Error 2.2: Not a valid WAV file (missing 'RIFF' or 'WAVE')");
        fclose(wav_file);
        return -1;
    }

    /*========================================*/
    /*        3. Search for fmt chunk         */
    /*========================================*/

    char chunk_id[4];
    uint32_t chunk_size;
    int fmt_found = false;
    FmtChunk fmt_chunk;

    // Process 3.1: WAV files can have chunks in any order, so we search for exactly "fmt "
    while (fmt_found == false)
    {
        // Error 3.1 wav_reader.c: Can't read chunk ID (file corrupted)
        if (fread(chunk_id, 4, 1, wav_file) != 1)
        {
            print_error("wav_reader.c", "Error 3.1: Can't read chunk ID (file corrupted)");
            fclose(wav_file);
            return -1;
        }

        // Error 3.2 wav_reader.c: Can't read chunk size (file corrupted)
        if (fread(&chunk_size, 4, 1, wav_file) != 1)
        {
            print_error("wav_reader.c", "Error 3.2: Can't read chunk size (file corrupted)");
            fclose(wav_file);
            return -1;
        }

        // Process 3.2: Checks if this is the format chunk
        if (strncmp(chunk_id, "fmt ", 4) == 0)
        {
            fmt_found = true;

            // Error 3.3 wav_reader.c: Can't read format data (file corrupted)
            if (fread(&fmt_chunk.audio_format, FMT_CHUNK_DATA_SIZE, 1, wav_file) != 1)
            {
                print_error("wav_reader.c", "Error 3.3: Can't read format data (file corrupted)");
                fclose(wav_file);
                return -1;
            }

            // Process 3.3: Chunk extra junk data skipping
            if (chunk_size > FMT_CHUNK_DATA_SIZE)
            {
                fseek(wav_file, chunk_size - FMT_CHUNK_DATA_SIZE, SEEK_CUR);
            }
        }
        
        // Process 3.4: Isn't the fmt chunk, we skip it
        else
        {
            
            fseek(wav_file, chunk_size, SEEK_CUR);
        }
    }

    /*========================================*/
    /*     4. Search for data chunk           */
    /*========================================*/

    int data_found = false;

    // Process 4.1: Search for the "data" chunk
    while (data_found == false)
    {
        // Error 4.1 wav_reader.c: Can't read chunk ID (file corrupted)
        if (fread(chunk_id, 4, 1, wav_file) != 1)
        {
            print_error("wav_reader.c", "Error 4.1: Can't read chunk ID (file corrupted)");
            fclose(wav_file);
            return -1;
        }

        // Error 4.2 wav_reader.c: Can't read chunk size (file corrupted)
        if (fread(&chunk_size, 4, 1, wav_file) != 1)
        {
            print_error("wav_reader.c", "Error 4.2: Can't read chunk size (file corrupted)");
            fclose(wav_file);
            return -1;
        }

        // Process 4.2: Check if this is the data chunk
        if (strncmp(chunk_id, "data", 4) == 0)
        {
            data_found = true;
            info->sample_rate = fmt_chunk.sample_rate;
            info->bit_depth = fmt_chunk.bit_depth;
            info->num_channels = fmt_chunk.num_channels;
            info->data_size = chunk_size;
        }
        // Process 4.3: Not the data chunk, again skiparoo
        else
        {
            fseek(wav_file, chunk_size, SEEK_CUR);
        }
    }

    fclose(wav_file);
    return 0;

}