#ifndef DOUBLE_ARRAY_H
#define DOUBLE_ARRAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef double* DoubleIterator;

typedef struct {

    size_t size;

    size_t capacity;

    double* data;

} DoubleArray;

#ifdef __cplusplus
}
#endif

#endif
