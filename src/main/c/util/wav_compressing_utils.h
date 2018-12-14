#ifndef WAV_COMPRESSING_UTILS_H
#define WAV_COMPRESSING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include <domain/compressed_data.h>
#include <domain/wav_file.h>
#include <util/decibel_utils.h>

typedef enum SilenceThresholdFilteredValue {
    STFV_BELOW = 0,
    STFV_ABOVE,
    STFV_INIT
} Stfv;

typedef struct {

    bool _is_initialized;

    void (*compress_int16_array)(Rsp* data, size_t data_size, bool start_chunk,
                                 Rsp threshold, Stfv* filtered_value, size_t* samples_count, CompressedData* dst);

    /**
     * Before using this function you MUST set up these
     *
     * Usage:
     *  CompressedData cdata = { .compressed_data=uint64_array_api().init(0) };
     *
     *  int16_t chunks[N_CHUNKS][CHUNK_SIZE] = {...};
     *
     *  int16_t threshold = ...;
     *
     *  size_t silence = ...;
     *
     *  bool fill = false;
     *  STF prev = STFV_INIT; // Within the function body `prev` can either be 1 or 0,
     *                       // so for initial state we set this to "safe" value.
     *  size_t last = 0;
     *
     *  for (int i = 0; i < N_CHUNKS; i++) {
     *      wav_compressing_utils()->compress_smooth_int16_array(chunks[i], CHUNK_SIZE, (i == 0), &fill,
     *                                                           threshold, silence, &prev, &last, &cdata);
     *  }
     *
     *  if (last > 0) {
     *      if (fill) {
     *          *(uint64_array_api().end(cdata.compressed_data) - 1) += last;
     *      } else {
     *          uint64_array_api().push_back(cdata.compressed_data, last);
     *      }
     *  }
     */
    void (*compress_smooth_int16_array)(Rsp* data, size_t data_size, bool start_chunk, bool* smooth_region_end,
                                        Rsp threshold, size_t silence, Stfv* filtered_value,
                                        size_t* samples_count, CompressedData* dst);

} WavCompressingUtils;

WavCompressingUtils* wav_compressing_utils();

#endif
