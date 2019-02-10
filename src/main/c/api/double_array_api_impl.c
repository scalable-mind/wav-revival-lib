#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <stdbool.h>

#define MODULE_API_EXPORTS
#include <domain/double_array.h>
#include <api/double_array_api.h>

#define CAPACITY_INCREASE_FACTOR 1.5
#define DOUBLE_ARRAY_MIN_CAPACITY 64

static DoubleArray* init(size_t capacity) {
    if (capacity < DOUBLE_ARRAY_MIN_CAPACITY) {
        capacity = DOUBLE_ARRAY_MIN_CAPACITY;
    }
    DoubleArray* array = malloc(sizeof(DoubleArray));
    array->_capacity = capacity;
    array->_data = calloc(capacity, sizeof(double));
    array->_size = 0;

    return array;
}

static void del(DoubleArray* self) {
    free(self->_data);
    free(self);
}

static void shrink_to_fit(DoubleArray* self) {
    size_t new_capacity = self->_size;
    if (new_capacity < DOUBLE_ARRAY_MIN_CAPACITY) {
        new_capacity = DOUBLE_ARRAY_MIN_CAPACITY;
    }
    if (self->_capacity > new_capacity) {
        void* ptr = calloc(new_capacity, sizeof(double));
        memcpy(ptr, self->_data, sizeof(double) * self->_size);
        free(self->_data);
        self->_data = ptr;
        self->_capacity = new_capacity;
    }
}

static void resize(DoubleArray* self, size_t new_size) {
    if (new_size < DOUBLE_ARRAY_MIN_CAPACITY) {
        new_size = DOUBLE_ARRAY_MIN_CAPACITY;
    }
    void* ptr = calloc(new_size, sizeof(double));
    memcpy(ptr, self->_data, (sizeof(double) * (new_size > self->_size ? self->_size : new_size)));
    free(self->_data);
    self->_data = ptr;
    self->_capacity = new_size;
    self->_size = new_size > self->_size ? self->_size : new_size;
}

static void push_back(DoubleArray* self, double val) {
    if (self->_size >= self->_capacity) {
        resize(self, (size_t) (CAPACITY_INCREASE_FACTOR * self->_capacity));
    }
    self->_data[self->_size++] = val;
}

static void push_some(DoubleArray* self, size_t count, double val) {
    for (size_t i = 0; i < count; i++) {
        push_back(self, val);
    }
}

static DoubleIterator begin(DoubleArray* self) {
    return &(self->_data[0]);
}

static DoubleIterator end(DoubleArray* self) {
    return begin(self) + self->_size;
}

static void print(DoubleArray* self) {
    printf("[ ");

    bool first = true;

    for (DoubleIterator it = begin(self); it != end(self); it++) {
        if (first) {
            printf("%.2lf", *it);
            first = false;
        }
        else {
            printf(", %.2lf", *it);
        }
    }

    printf(" ]\n");
}

DoubleArrayApi* double_array_api() {
    static DoubleArrayApi instance = { ._is_initialized=false };

    if (!instance._is_initialized) {
        instance._is_initialized = true;
        instance.init = init;
        instance.del = del;
        instance.push_back = push_back;
        instance.push_some = push_some;
        instance.shrink_to_fit = shrink_to_fit;
        instance.resize = resize;
        instance.begin = begin;
        instance.end = end;
        instance.print = print;
    }

    return &instance;
}

#undef DOUBLE_ARRAY_MIN_CAPACITY
#undef CAPACITY_INCREASE_FACTOR
#undef MODULE_API_EXPORTS
