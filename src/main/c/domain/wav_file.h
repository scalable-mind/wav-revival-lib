#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <stdio.h>

#include "./wav_header.h"

typedef enum {

    READ,

    WRITE

} FileIoMode;

typedef struct {

    WavHeader header;

    size_t buffer_size;

    FILE* file;

} WavFile;

#endif
