#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "field_info.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct DynamicArray {
    void* data;
    size_t capacity;
    size_t curr_size;
    const FieldInfo* el_type;
} DynamicArray;

DynamicArray* arr_create(const FieldInfo* type, size_t start_capacity);
void arr_destroy(DynamicArray* arr);

void* get_elem_arr(const DynamicArray* arr, const size_t index);
bool set_elem_arr(DynamicArray* arr, const size_t index, const void* value);

bool arr_add_el(DynamicArray* arr, const size_t index, const void* value);
bool arr_remove_el(DynamicArray* arr, const size_t index);

#endif DYNAMIC_ARRAY_H