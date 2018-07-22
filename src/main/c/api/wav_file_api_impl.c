#include <stdlib.h>
#include <stdbool.h>

#include "./wav_file_api.h"
#include "../domain/wav_file.h"

static WavFile* init(const char* file_path, FileIoMode file_io_mode, size_t buffer_size) {
    WavFile* wav_file = malloc(sizeof(WavFile));
    wav_file->buffer_size = buffer_size;
    //TODO: file_io_mode validation
    errno_t err = fopen_s(&(wav_file->file), file_path, file_io_mode == READ ? "rb" : "wb");
    //TODO: errno validation
    fread(&(wav_file->header), sizeof(WavHeader), 1, wav_file->file);
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

WavFileApi wav_file_api() {
    static WavFileApi instance;
    static bool is_initialized = false;

    if (!is_initialized) {
        is_initialized = true;
        instance.init = init;
        instance.del = del;
        instance.read_next_chunk = read_next_chunk;
        instance.write_next_chunk = write_next_chunk;
    }
    return instance;
}
