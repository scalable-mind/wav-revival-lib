#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "string.h"

#define MODULE_API_EXPORTS

#include <api/wav_file_api.h>
#include <domain/wav_file.h>

static WavFile* init(const char* file_path, const char* file_io_mode, size_t buffer_size) {
    WavFile* wav_file = malloc(sizeof(WavFile));
    wav_file->buffer_size = buffer_size;
#if _WIN32 || _WIN64
    //TODO: file_io_mode validation
    //TODO: errno validation
    errno_t err = fopen_s(&(wav_file->file), file_path, file_io_mode);
#else
    wav_file->file = fopen(file_path, file_io_mode);
#endif
    if (strcmp(file_io_mode, "r") == 0) {
        fread(&(wav_file->header), sizeof(WavHeader), 1, wav_file->file);
    }
    return wav_file;
}

static void del(WavFile* self) {
    fclose(self->file);
    free(self);
}

static size_t read_next_chunk(WavFile* self, void* buffer) {
    size_t sample_size = self->header.block_align / self->header.num_channels;
    return fread(buffer, sample_size, self->buffer_size, self->file);
}

static size_t write_next_chunk(WavFile* self, void* buffer) {
    size_t sample_size = self->header.block_align / self->header.num_channels;
    return fwrite(buffer, sample_size, self->buffer_size, self->file);
}

static size_t write_header(WavFile* self, WavHeader* header) {
    return fwrite(header, sizeof(WavHeader), 1, self->file);
}

static void refresh(WavFile* self) {
    fseek(self->file, sizeof(WavHeader), SEEK_SET);
}

WavFileApi* wav_file_api() {
    static WavFileApi instance = {._is_initialized=false};

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.init = init;
        instance.del = del;
        instance.read_next_chunk = read_next_chunk;
        instance.write_next_chunk = write_next_chunk;
        instance.refresh = refresh;
        instance.write_header = write_header;
    }

    return &instance;
}

#undef MODULE_API_EXPORTS
