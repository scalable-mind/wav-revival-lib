#ifndef WAV_FILE_API_H
#define WAV_FILE_API_H

#include <domain/wav_file.h>

#include <macros/export_start.h>

typedef struct {

    bool _is_initialized;

    WavFile* (*init)(const char* file_path, FileIoMode file_io_mode, size_t buffer_size);

    void (*del)(WavFile* self);

    size_t (*read_next_chunk)(WavFile* self, void* buffer);

    size_t (*write_next_chunk)(WavFile* self, void* buffer);

} WavFileApi;

MODULE_API WavFileApi* wav_file_api();

#include <macros/export_end.h>

#endif
