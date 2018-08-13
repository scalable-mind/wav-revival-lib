#ifndef WAV_FILE_API_H
#define WAV_FILE_API_H

#include <domain/wav_file.h>

typedef struct {

    WavFile* (*init)(const char* file_path, FileIoMode file_io_mode, size_t buffer_size);

    void (*del)(WavFile* self);

    size_t (*read_next_chunk)(WavFile* self, void* buffer);

    size_t (*write_next_chunk)(WavFile* self, void* buffer);

    void (*refresh)(WavFile* self);

    size_t (*write_header)(WavFile* self, WavHeader* header);

} WavFileApi;

WavFileApi wav_file_api();

#endif
