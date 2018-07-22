#include <stdlib.h>

#include "./sample_count_array_api.h"
#include "../domain/sample_count_array.h"

#define CAPACITY_INCREASE_FACTOR 1.5

static SampleCountArray* init(size_t capacity) {
    SampleCountArray* array = malloc(sizeof(SampleCountArray));
    array->capacity = capacity;
    array->data = calloc(capacity, sizeof(size_t));
    array->size = 0;
}

static void del(SampleCountArray* self) {
    free(self->data);
    free(self);
}

static void shrink_to_fit(SampleCountArray* self) {
    if (self->capacity > self->size) {
        void* ptr = calloc(self->size, sizeof(size_t));
        memcpy(ptr, self->data, sizeof(size_t) * self->size);
        free(self->data);
        self->data = ptr;
        self->capacity = self->size;
    }
}

static void resize(SampleCountArray* self, size_t new_size) {
    void* ptr = calloc(new_size, sizeof(size_t));
    memcpy(ptr, self->data, (sizeof(size_t) * (new_size > self->size ? self->size : new_size)));
    free(self->data);
    self->data = ptr;
    self->capacity = new_size;
    self->size = new_size > self->size ? self->size : new_size;
}

static void push_back(SampleCountArray* self, size_t val) {
    if (self->size >= self->capacity) {
        resize(self, CAPACITY_INCREASE_FACTOR * self->capacity);
    }
    self->data[self->size++] = val;
}

static sca_iterator begin(SampleCountArray* self) {
    return &(self->data[0]);
}

static sca_iterator end(SampleCountArray* self) {
    return begin(self) + self->size;
}

SampleCountArrayApi sample_count_array_api() {
    static SampleCountArrayApi instance;
    static bool is_initialized = false;

    if (!is_initialized) {
        is_initialized = true;
        instance.init = init;
        instance.del = del;
        instance.push_back = push_back;
        instance.shrink_to_fit = shrink_to_fit;
        instance.resize = resize;
        instance.begin = begin;
        instance.end = end;
    }
    return instance;
}
