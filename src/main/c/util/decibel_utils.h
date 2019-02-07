#ifndef DECIBEL_UTILS_H
#define DECIBEL_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#include <macros/export_start.h>

/** Signal amplitude */
typedef int16_t Amp;

typedef double Decibel;

typedef struct {

    bool _is_initialized;

    Decibel (*get_spl)(Amp gain);

    Amp (*spl_to_rsp)(Decibel gain);

    double (*spl_to_ratio)(Decibel gain);

} DecibelUtils;

MODULE_API DecibelUtils* decibel_utils();

#include <macros/export_end.h>

#endif
