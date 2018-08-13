#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include <domain/uint64_array.h>
#include <api/uint64_array_api.h>

#define CAPACITY_INCREASE_FACTOR 1.5
#define UINT64_ARRAY_MIN_CAPACITY 64

static Uint64Array* init(size_t capacity) {
    if (capacity < UINT64_ARRAY_MIN_CAPACITY) {
        capacity = UINT64_ARRAY_MIN_CAPACITY;
    }
    Uint64Array* array = malloc(sizeof(Uint64Array));
    array->capacity = capacity;
    array->data = calloc(capacity, sizeof(uint64_t));
    array->size = 0;

    return array;
}

static void del(Uint64Array* self) {
    free(self->data);
    free(self);
}

static void shrink_to_fit(Uint64Array* self) {
    size_t new_capacity = self->size;
    if (new_capacity < UINT64_ARRAY_MIN_CAPACITY) {
        new_capacity = UINT64_ARRAY_MIN_CAPACITY;
    }
    if (self->capacity > new_capacity) {
        void* ptr = calloc(new_capacity, sizeof(uint64_t));
        memcpy(ptr, self->data, sizeof(uint64_t) * self->size);
        free(self->data);
        self->data = ptr;
        self->capacity = new_capacity;
    }
}

static void resize(Uint64Array* self, size_t new_size) {
    if (new_size < UINT64_ARRAY_MIN_CAPACITY) {
        new_size = UINT64_ARRAY_MIN_CAPACITY;
    }
    void* ptr = calloc(new_size, sizeof(uint64_t));
    memcpy(ptr, self->data, (sizeof(uint64_t) * (new_size > self->size ? self->size : new_size)));
    free(self->data);
    self->data = ptr;
    self->capacity = new_size;
    self->size = new_size > self->size ? self->size : new_size;
}

static void push_back(Uint64Array* self, uint64_t val) {
    if (self->size >= self->capacity) {
        resize(self, (uint64_t) (CAPACITY_INCREASE_FACTOR * self->capacity));
    }
    self->data[self->size++] = val;
}

static void push_some(Uint64Array* self, size_t count, uint64_t val) {
    for (size_t i = 0; i < count; i++) {
        push_back(self, val);
    }
}

static Uint64Iterator begin(Uint64Array* self) {
    return &(self->data[0]);
}

static Uint64Iterator end(Uint64Array* self) {
    return begin(self) + self->size;
}

Uint64ArrayApi uint64_array_api() {
    static Uint64ArrayApi instance;
    static bool is_initialized = false;

    if (!is_initialized) {
        is_initialized = true;
        instance.init = init;
        instance.del = del;
        instance.push_back = push_back;
        instance.push_some = push_some;
        instance.shrink_to_fit = shrink_to_fit;
        instance.resize = resize;
        instance.begin = begin;
        instance.end = end;
    }
    return instance;
}
