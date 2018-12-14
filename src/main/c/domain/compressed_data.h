#ifndef WAV_COMPRESSED_DATA_H
#define WAV_COMPRESSED_DATA_H

#include <domain/uint64_array.h>

typedef struct {

    int start_value;

    Uint64Array* compressed_data;

} CompressedData;

#endif
