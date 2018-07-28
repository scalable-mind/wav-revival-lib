#ifndef SAMPLE_COUNT_ARRAY_API_H
#define SAMPLE_COUNT_ARRAY_API_H

#include "../domain/sample_count_array.h"

typedef struct {

    SampleCountArray* (*init)(size_t capacity);

    void (*del)(SampleCountArray* self);

    void (*shrink_to_fit)(SampleCountArray* self);

    void (*resize)(SampleCountArray* self, size_t new_size);

    void (*push_back)(SampleCountArray* self, size_t val);

    sca_iterator (*begin)(SampleCountArray* self);

    sca_iterator (*end)(SampleCountArray* self);

} SampleCountArrayApi;

SampleCountArrayApi sample_count_array_api();

#endif
