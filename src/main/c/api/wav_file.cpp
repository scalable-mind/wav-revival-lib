#include <cerrno>
#include <string>
#include <cstdio>

#include "../domain/wav_file.hpp"

WavFile::WavFile(
        const std::string &file_path,
        const std::string &file_io_mode,
        size_t buffer_size) : buffer_size(buffer_size) {
#if _MSC_VER
    //TODO: file_io_mode validation
    //TODO: errno validation
    errno_t err = fopen_s(&(file), file_path.c_str(), file_io_mode.c_str());
#else
    file = fopen(file_path.c_str(), file_io_mode.c_str());
#endif
    if (file_io_mode != "rb") {
        fread(&(header), sizeof(WavHeader), 1, file);
        this->buffer_size *= header.num_channels;
    }
}

WavFile::~WavFile() {
    fclose(file);
}

size_t WavFile::read_next_chunk(void *buffer) const {
    size_t sample_size = header.block_align / header.num_channels;
    return fread(buffer, sample_size, buffer_size, file);
}

size_t WavFile::write_next_chunk(void *buffer, size_t size) const {
    size_t sample_size = header.block_align / header.num_channels;
    return fwrite(buffer, sample_size, size, file);
}

size_t WavFile::write_header(WavHeader *wav_header) {
    this->header = *wav_header;
    buffer_size *= wav_header->num_channels;
    return fwrite(wav_header, sizeof(WavHeader), 1, file);
}

void WavFile::refresh() const {
    fseek(file, sizeof(WavHeader), SEEK_SET);
}
