#ifndef DOUBLE_ARRAY_API_H
#define DOUBLE_ARRAY_API_H

#include <stdbool.h>

#include "../domain/double_array.h"

#include "../macros/export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    bool _is_initialized;

    DoubleArray* (*init)(size_t capacity);

    void (*del)(DoubleArray* self);

    void (*shrink_to_fit)(DoubleArray* self);

    void (*resize)(DoubleArray* self, size_t new_size);

    void (*push_back)(DoubleArray* self, double val);

    void (*push_some)(DoubleArray* self, size_t count, double val);

    DoubleIterator (*begin)(DoubleArray* self);

    DoubleIterator (*end)(DoubleArray* self);

    void (*print)(DoubleArray* self);

} DoubleArrayApi;

MODULE_API DoubleArrayApi* double_array_api();

#ifdef __cplusplus
}
#endif

#endif
