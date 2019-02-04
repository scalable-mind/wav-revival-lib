#ifndef DECIBEL_UTILS_H
#define DECIBEL_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#include <macros/export_start.h>

/** RSP is Raw Sound Pressure, a digital signal representation of sound pressure in pascal (Pa). */
typedef int16_t Rsp;

typedef double Decibel;

typedef struct {

    bool _is_initialized;

    Decibel (*get_spl)(Rsp gain);

    Rsp (*spl_to_rsp)(Decibel gain);

    double (*spl_to_ratio)(Decibel gain);

} DecibelUtils;

MODULE_API DecibelUtils* decibel_utils();

#include <macros/export_end.h>

#endif
