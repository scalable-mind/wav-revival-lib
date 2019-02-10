#ifndef WAV_COMPRESSED_DATA_H
#define WAV_COMPRESSED_DATA_H

#include "../domain/uint64_array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    int start_value;

    Uint64Array* compressed_data;

} CompressedData;

#ifdef __cplusplus
}
#endif

#endif
