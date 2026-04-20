#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "field_info.h"

typedef struct DynamicArray {
    void* data;
    size_t capacity;
    size_t curr_size;
    const FieldInfo* el_type;
} DynamicArray;


DynamicArray* arr_create(const FieldInfo* type, size_t start_size);
void arr_destroy(DynamicArray *arr);

void* get_elem_arr(const DynamicArray *arr, const size_t index);
void set_elem_arr(DynamicArray *arr, const size_t index, const void* value);

void arr_add_el(DynamicArray *arr, const size_t index, const void* value);
void arr_remove_el(DynamicArray* arr, const size_t index);
void arr_resize(DynamicArray *arr, const size_t new_size);

void arr_clear(DynamicArray *arr);
void arr_print(const DynamicArray *arr);

#endif DYNAMIC_ARRAY_H