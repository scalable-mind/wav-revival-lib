#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#define MODULE_API_EXPORTS

#include <domain/uint64_array.h>
#include <api/uint64_array_api.h>

#define CAPACITY_INCREASE_FACTOR 1.5
#define UINT64_ARRAY_MIN_CAPACITY 64

static Uint64Array* init(size_t capacity) {
    if (capacity < UINT64_ARRAY_MIN_CAPACITY) {
        capacity = UINT64_ARRAY_MIN_CAPACITY;
    }
    Uint64Array* array = malloc(sizeof(Uint64Array));
    array->_capacity = capacity;
    array->_data = calloc(capacity, sizeof(uint64_t));
    array->_size = 0;

    return array;
}

static void del(Uint64Array* self) {
    free(self->_data);
    free(self);
}

static void shrink_to_fit(Uint64Array* self) {
    size_t new_capacity = self->_size;
    if (new_capacity < UINT64_ARRAY_MIN_CAPACITY) {
        new_capacity = UINT64_ARRAY_MIN_CAPACITY;
    }
    if (self->_capacity > new_capacity) {
        void* ptr = calloc(new_capacity, sizeof(uint64_t));
        memcpy(ptr, self->_data, sizeof(uint64_t) * self->_size);
        free(self->_data);
        self->_data = ptr;
        self->_capacity = new_capacity;
    }
}

static void resize(Uint64Array* self, size_t new_size) {
    if (new_size < UINT64_ARRAY_MIN_CAPACITY) {
        new_size = UINT64_ARRAY_MIN_CAPACITY;
    }
    void* ptr = calloc(new_size, sizeof(uint64_t));
    memcpy(ptr, self->_data, (sizeof(uint64_t) * (new_size > self->_size ? self->_size : new_size)));
    free(self->_data);
    self->_data = ptr;
    self->_capacity = new_size;
    self->_size = new_size > self->_size ? self->_size : new_size;
}

static void push_back(Uint64Array* self, uint64_t val) {
    if (self->_size >= self->_capacity) {
        resize(self, (size_t) (CAPACITY_INCREASE_FACTOR * self->_capacity));
    }
    self->_data[self->_size++] = val;
}

static Uint64Iterator begin(Uint64Array* self) {
    return &(self->_data[0]);
}

static Uint64Iterator end(Uint64Array* self) {
    return begin(self) + self->_size;
}

Uint64ArrayApi* uint64_array_api() {
    static Uint64ArrayApi instance = { ._is_initialized=false };

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.init = init;
        instance.del = del;
        instance.push_back = push_back;
        instance.shrink_to_fit = shrink_to_fit;
        instance.resize = resize;
        instance.begin = begin;
        instance.end = end;
    }

    return &instance;
}

#undef UINT64_ARRAY_MIN_CAPACITY
#undef CAPACITY_INCREASE_FACTOR
#undef MODULE_API_EXPORTS
