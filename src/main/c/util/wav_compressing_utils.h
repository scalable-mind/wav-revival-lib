#ifndef WAV_COMPRESSING_UTILS_H
#define WAV_COMPRESSING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "../domain/compressed_data.h"
#include "../domain/wav_file.h"
#include "../util/decibel_utils.h"

#include "../macros/export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SilenceThresholdFilteredValue {
    STFV_BELOW = 0,
    STFV_ABOVE,
    STFV_INIT
} Stfv;

typedef struct {

    bool _is_initialized;

    /**
     * Usage:
     *  CompressedData cdata = { .compressed_data=uint64_array_api().init(0) };
     *
     *  Amp chunks[N_CHUNKS][CHUNK_SIZE] = {...};
     *
     *  Amp threshold = ...;
     *
     *  size_t silence = ...;
     *
     *  bool smooth_region_end = false;
     *  Stfv filtered_value = STFV_INIT; // Within the function body `filtered_value` can either be  or 0,
     *                                   // so for initial state we set this to "STFV_INIT".
     *  size_t last = 0;
     *
     *  for (int i = 0; i < N_CHUNKS; i++) {
     *      wav_compressing_utils()->compress_smooth_rsp_chunk(chunks[i], CHUNK_SIZE, (i == 0), &smooth_region_end,
     *                                                         threshold, silence, &filtered_value, &last, &cdata);
     *  }
     *
     *  if (last > 0) {
     *      if (smooth_region_end) {
     *          *(uint64_array_api().end(cdata.compressed_data) - 1) += last;
     *      } else {
     *          uint64_array_api().push_back(cdata.compressed_data, last);
     *      }
     *  }
     */
    void (*compress_smooth_rsp_chunk)(Amp* data, size_t data_size, bool start_chunk, bool* smooth_region_end,
                                      Amp threshold, size_t silence, Stfv* filtered_value,
                                      size_t* samples_count, CompressedData* dst);

} WavCompressingUtils;

MODULE_API WavCompressingUtils* wav_compressing_utils();

#ifdef __cplusplus
}
#endif

#endif
