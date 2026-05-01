#ifndef STRING_COLLECTION_H
#define STRING_COLLECTION_H

#include "field_info.h"
#include "dynamic_array.h"

DynamicArray* str_create_from_cstring(const char* source);
DynamicArray* str_to_char(const DynamicArray* str_arr);
char* str_to_cstring(const DynamicArray* char_arr);

DynamicArray* str_split_to_words(const DynamicArray* char_arr);
DynamicArray* str_concat(const DynamicArray* arr1, const DynamicArray* arr2);
DynamicArray* str_sub_range(const DynamicArray* arr, const size_t start, const size_t end);

DynamicArray* str_trim_spaces(const DynamicArray* char_arr);
DynamicArray* str_to_upper(const DynamicArray* char_arr);
DynamicArray* str_to_lower(const DynamicArray* char_arr);

void str_print(const DynamicArray* arr);

#endif