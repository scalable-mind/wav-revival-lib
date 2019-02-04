#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <stdio.h>

#include <domain/wav_header.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {

    READ,

    WRITE

} FileIoMode;

typedef struct {

    WavHeader header;

    size_t buffer_size;

    FILE* file;

} WavFile;

#ifdef __cplusplus
}
#endif

#endif
