#ifndef DECIBEL_UTILS_H
#define DECIBEL_UTILS_H

#include <stdint.h>
#include <stdbool.h>

typedef int16_t Pascal;

typedef double Decibel;

typedef struct {

    bool _is_initialized;

    Decibel (*get_spl)(Pascal gain);

    Pascal (*spl_to_pascal)(Decibel gain);

    double (*spl_to_ratio)(Decibel gain);

} DecibelUtils;

DecibelUtils* decibel_utils();

#endif
