#ifndef DOUBLE_ARRAY_H
#define DOUBLE_ARRAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef double* DoubleIterator;

typedef struct {

    size_t _size;

    size_t _capacity;

    double* _data;

} DoubleArray;

#ifdef __cplusplus
}
#endif

#endif
