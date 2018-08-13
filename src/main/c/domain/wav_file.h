#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <stdio.h>

#include <domain/wav_header.h>

#define KB_1 1024
#define MB_1 KB_1*1024
#define GB_1 MB_1*1024

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
