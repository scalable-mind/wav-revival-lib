#ifndef WAV_REVIVAL_H
#define WAV_REVIVAL_H

#include "../macros/export.h"

#ifdef __cplusplus
extern "C" {
#endif

MODULE_API void voice_over(
        const char* orig_path,
        const char* voice_path,
        const char* output_path,
        Amp threshold,
        size_t silence,
        size_t attack,
        size_t release,
        Decibel gain);

#ifdef __cplusplus
}
#endif

#endif
