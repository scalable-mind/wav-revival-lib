#ifndef UINT64_ARRAY_API_H
#define UINT64_ARRAY_API_H

#include <domain/uint64_array.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    bool _is_initialized;

    Uint64Array* (*init)(size_t capacity);

    void (*del)(Uint64Array* self);

    void (*shrink_to_fit)(Uint64Array* self);

    void (*resize)(Uint64Array* self, size_t new_size);

    void (*push_back)(Uint64Array* self, uint64_t val);

    Uint64Iterator (*begin)(Uint64Array* self);

    Uint64Iterator (*end)(Uint64Array* self);

} Uint64ArrayApi;

Uint64ArrayApi* uint64_array_api();

#ifdef __cplusplus
}
#endif

#endif
