#ifndef SAMPLE_COUNT_ARRAY_H
#define SAMPLE_COUNT_ARRAY_H
#define SAMPLE_COUNT_ARRAY_MIN_CAPACITY 64

#include <stdint.h>

typedef size_t* sca_iterator;

typedef struct {

    size_t size;

    size_t capacity;

    size_t* data;

} SampleCountArray;

#endif
