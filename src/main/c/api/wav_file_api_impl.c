#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <api/wav_file_api.h>
#include <domain/wav_file.h>

static WavFile* init(const char* file_path, FileIoMode file_io_mode, size_t buffer_size) {
    WavFile* wav_file = malloc(sizeof(WavFile));
    //TODO: file_io_mode validation
    errno_t err = fopen_s(&(wav_file->file), file_path, file_io_mode == READ ? "rb" : "wb");
    if (err != 0) exit(err);
    //TODO: errno validation
    if (file_io_mode == READ) {
        fread(&(wav_file->header), sizeof(WavHeader), 1, wav_file->file);
        wav_file->buffer_size = buffer_size * wav_file->header.num_channels;
    } else {
        wav_file->buffer_size = buffer_size;
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

WavFileApi wav_file_api() {
    static WavFileApi instance;
    static bool is_initialized = false;

    if (!is_initialized) {
        is_initialized = true;
        instance.init = init;
        instance.del = del;
        instance.read_next_chunk = read_next_chunk;
        instance.write_next_chunk = write_next_chunk;
        instance.refresh = refresh;
        instance.write_header = write_header;
    }
    return instance;
}
