#ifndef UINT64_ARRAY_H
#define UINT64_ARRAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t* Uint64Iterator;

typedef struct {

    size_t size;

    size_t capacity;

    uint64_t* data;

} Uint64Array;

#ifdef __cplusplus
}
#endif

#endif
