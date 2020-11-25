#pragma once

#include <cstdio>
#include <string>

#include "../domain/wav_header.h"

class WavFile {
public:
    WavHeader header{};

    size_t buffer_size;

    FILE *file;

    WavFile(const std::string &file_path, const std::string &file_io_mode, size_t buffer_size);

    ~WavFile();

    size_t read_next_chunk(void *buffer) const;

    size_t write_next_chunk(void *buffer, size_t size) const;

    void refresh() const;

    size_t write_header(WavHeader *wav_header);
};
