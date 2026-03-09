#ifndef WAV_READER_H
#define WAV_READER_H

#include <stdint.h>

typedef struct {
    uint32_t sample_rate;
    uint16_t bit_depth;
    uint16_t num_channels;
    uint32_t data_size;
} WavInfo;

int read_wav_info(const char *filename, WavInfo *info);

#endif