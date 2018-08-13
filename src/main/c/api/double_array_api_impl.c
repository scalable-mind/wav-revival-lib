#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include <domain/double_array.h>
#include <api/double_array_api.h>

#define CAPACITY_INCREASE_FACTOR 1.5
#define DOUBLE_ARRAY_MIN_CAPACITY 64

static DoubleArray* init(size_t capacity) {
    if (capacity < DOUBLE_ARRAY_MIN_CAPACITY) {
        capacity = DOUBLE_ARRAY_MIN_CAPACITY;
    }
    DoubleArray* array = malloc(sizeof(DoubleArray));
    array->capacity = capacity;
    array->data = calloc(capacity, sizeof(double));
    array->size = 0;

    return array;
}

static void del(DoubleArray* self) {
    free(self->data);
    free(self);
}

static void shrink_to_fit(DoubleArray* self) {
    size_t new_capacity = self->size;
    if (new_capacity < DOUBLE_ARRAY_MIN_CAPACITY) {
        new_capacity = DOUBLE_ARRAY_MIN_CAPACITY;
    }
    if (self->capacity > new_capacity) {
        void* ptr = calloc(new_capacity, sizeof(double));
        memcpy(ptr, self->data, sizeof(double) * self->size);
        free(self->data);
        self->data = ptr;
        self->capacity = new_capacity;
    }
}

static void resize(DoubleArray* self, size_t new_size) {
    if (new_size < DOUBLE_ARRAY_MIN_CAPACITY) {
        new_size = DOUBLE_ARRAY_MIN_CAPACITY;
    }
    void* ptr = calloc(new_size, sizeof(double));
    memcpy(ptr, self->data, (sizeof(double) * (new_size > self->size ? self->size : new_size)));
    free(self->data);
    self->data = ptr;
    self->capacity = new_size;
    self->size = new_size > self->size ? self->size : new_size;
}

static void push_back(DoubleArray* self, double val) {
    if (self->size >= self->capacity) {
        resize(self, (size_t) (CAPACITY_INCREASE_FACTOR * self->capacity));
    }
    self->data[self->size++] = val;
}

static void push_some(DoubleArray* self, size_t count, double val) {
    for (size_t i = 0; i < count; i++) {
        push_back(self, val);
    }
}

static DoubleIterator begin(DoubleArray* self) {
    return &(self->data[0]);
}

static DoubleIterator end(DoubleArray* self) {
    return begin(self) + self->size;
}

DoubleArrayApi double_array_api() {
    static DoubleArrayApi instance;
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