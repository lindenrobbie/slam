#ifndef WAV_READER_H
#define WAV_READER_H

#include <stdint.h>

//Wave file consists of 3 parts:

// 1. WAV file header
typedef struct {
    char riff[4];
    uint32_t file_size;
    char wave[4];
} RiffHeader;

// 2. Metadata (Fmt chunk) (format chunk)
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

typedef struct {
    uint32_t sample_rate;
    uint16_t bit_depth;
    uint16_t num_channels;
    uint32_t data_size;
} WavInfo;

int read_wav_info(const char *filename, WavInfo *info);

#endif