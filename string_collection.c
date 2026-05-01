#include "field_info.h"
#include "types.h"
#include "string_collection.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static DynamicArray* str_change_case(const DynamicArray* char_arr, int mode){
    if (!char_arr){
        fprintf(stderr, "str_to_case error (NULL pointer received)\n");
        return NULL;
    }

    if (!is_types_equal(char_arr->el_type, get_char_info())){
        fprintf(stderr, "str_to_case error (arr must be char type)\n");
        return NULL;
    }

    DynamicArray* res_arr = arr_create(char_arr->el_type, char_arr->curr_size);
    if (!res_arr){
        fprintf(stderr, "str_to_case error (arr_create return NULL)\n");
        return NULL;
    }

    for (size_t i = 0; i < char_arr->curr_size; i++){
        char elem = *(char*)get_elem_arr(char_arr, i);
        char modified = (mode == 1) ? (char)toupper((unsigned char)elem) : (char)tolower((unsigned char)elem);
        arr_add_el(res_arr, res_arr->curr_size, &modified);
    }

    return res_arr;
}

DynamicArray* str_create_from_cstring(const char* source){
    if (!source){
        fprintf(stderr, "str_create_from_cstring error (NULL pointer received)\n");
        return NULL;
    }

    size_t str_len = strlen(source);
    DynamicArray* str_collection = arr_create(get_char_info(), str_len);
    if (!str_collection){
        fprintf(stderr, "str_create_from_cstring error (arr_create return NULL)\n");
        return NULL;
    }

    for (size_t i = 0; i < str_len; i++){
        if (!arr_add_el(str_collection, i, &source[i])){
            arr_destroy(str_collection);
            return NULL;
        }
    }

    return str_collection;
}

DynamicArray* str_to_char(const DynamicArray* str_arr){
    if (!str_arr){
        fprintf(stderr, "str_to_char error (NULL pointer received)\n");
        return NULL;
    }

    if (!is_types_equal(str_arr->el_type, get_string_info())){
        fprintf(stderr, "str_to_char error (argument must be string type)\n");
        return NULL;
    }

    DynamicArray* res_arr = arr_create(get_char_info(), str_arr->curr_size);
    if (!res_arr){
        fprintf(stderr, "str_to_char error (arr_create return NULL)\n");
        return NULL;
    }

    for (size_t i = 0; i < str_arr ->curr_size; i++){
        char* word = *(char**)get_elem_arr(str_arr, i);
        if (!word) continue;

        size_t word_len = strlen(word);
        for (size_t j = 0; j < word_len; j++){
            arr_add_el(res_arr, res_arr->curr_size, &word[j]);
        }

        if (i < str_arr->curr_size - 1){
            char space = ' ';
            arr_add_el(res_arr, res_arr->curr_size, &space);
        }
    }

    return res_arr;
}

char* str_to_cstring(const DynamicArray* char_arr){
    if (!char_arr){
        fprintf(stderr, "str_to_cstring error (NULL pointer received)\n");
        return NULL;
    }

    if (!is_types_equal(char_arr->el_type, get_char_info())){
        fprintf(stderr, "str_to_cstring error (argument must be string type)\n");
        return NULL;
    }

    char* res_str = (char*)malloc(char_arr->curr_size + 1);
    if (!res_str){
        fprintf(stderr, "str_to_cstring error (malloc failure)\n");
        return NULL;
    }

    for (size_t i = 0; i < char_arr->curr_size; i++) {
        res_str[i] = *(char*)get_elem_arr(char_arr, i);
    }

    res_str[char_arr->curr_size] = '\0';
    return res_str;
}

DynamicArray* str_split_to_words(const DynamicArray* char_arr){
    if (!char_arr){
        fprintf(stderr, "str_split_to_words error (NULL pointer received)\n");
        return NULL;
    }

    DynamicArray* res_arr = arr_create(get_string_info(), char_arr->curr_size);
    if (!res_arr){
        fprintf(stderr, "str_split_to_words error (arr_create return NULL)\n");
        return NULL;
    }

    char* word_buffer = (char*)malloc(char_arr->curr_size + 1);
    size_t buffer_index = 0;

    for (size_t i = 0; i < char_arr->curr_size; i++){
        void* current_char_ptr = get_elem_arr(char_arr, i);
        if (!char_arr->el_type->is_separator(current_char_ptr)){
            word_buffer[buffer_index] = *(char*)current_char_ptr;
            buffer_index++;
        }
        else{
            if (buffer_index > 0){
                word_buffer[buffer_index] = '\0';
                char* value = strdup(word_buffer);
                arr_add_el(res_arr, res_arr->curr_size, &value);
                buffer_index = 0;
            }
        }
    }

    if (buffer_index > 0){
        word_buffer[buffer_index] = '\0';
        char* value = strdup(word_buffer);
        arr_add_el(res_arr, res_arr->curr_size, &value);
    }
    free(word_buffer);

    return res_arr;
}

DynamicArray* str_concat(const DynamicArray* arr1, const DynamicArray* arr2){
    if (!arr1 || !arr2){
        fprintf(stderr, "str_concat error (NULL pointer received)\n");
        return NULL;
    }

    if (!is_types_equal(arr1->el_type, arr2->el_type)){
        fprintf(stderr, "str_concat error (arr1 and arr2 have different types: %s and %s)\n", arr1->el_type->type_name, arr2->el_type->type_name);
        return NULL;
    }

    DynamicArray* res_arr = arr_create(arr1->el_type, arr1->curr_size + arr2->curr_size);
    if (!res_arr){
        fprintf(stderr, "str_concat error (arr_create return NULL)\n");
        return NULL;
    }

    for (size_t i = 0; i < arr1->curr_size; i++){
        arr_add_el(res_arr, res_arr->curr_size, get_elem_arr(arr1, i));
    }
    for (size_t i = 0; i < arr2->curr_size; i++){
        arr_add_el(res_arr, res_arr->curr_size, get_elem_arr(arr2, i));
    }

    return res_arr;
}

DynamicArray* str_sub_range(const DynamicArray* arr, size_t start, size_t end){
    if (!arr){
        fprintf(stderr, "str_sub_range error (NULL pointer received)\n");
        return NULL;
    }

    size_t new_start = start;
    size_t new_end = end;

    if (start > arr->curr_size){
        new_start = arr->curr_size;
    }

    if (end > arr->curr_size){
        new_end = arr->curr_size;
    }

    if (new_start >= new_end) {
        fprintf(stderr, "str_sub_range warning (start >= end)\nReturning empty array\n");
        return arr_create(arr->el_type, 1); 
    }

    DynamicArray* res_arr = arr_create(arr->el_type, new_end - new_start);
    if (!res_arr){
        fprintf(stderr, "str_sub_range error (arr_create return NULL)\n");
        return NULL;
    }


    for (size_t i = new_start; i < new_end; i++){
        arr_add_el(res_arr, res_arr->curr_size, get_elem_arr(arr, i));
    }

    return res_arr;
}

DynamicArray* str_trim_spaces(const DynamicArray* char_arr){
    if (!char_arr){
        fprintf(stderr, "str_trim_spaces error (NULL pointer received)\n");
        return NULL;
    }

    if (!is_types_equal(char_arr->el_type, get_char_info())){
        fprintf(stderr, "str_trim_spaces error (argument must be char type)\n");
        return NULL;
    }

    DynamicArray* res_arr = arr_create(char_arr->el_type, char_arr->curr_size);
    if (!res_arr){
        fprintf(stderr, "str_trim_spaces error (arr_create return NULL)\n");
        return NULL;
    }

    size_t start = 0;
    size_t end = char_arr->curr_size;

    while (start < end && char_arr->el_type->is_separator(get_elem_arr(char_arr, start))){
        start++;
    }
    while (end > start && char_arr->el_type->is_separator(get_elem_arr(char_arr, end - 1))){
        end--;
    }

    bool last_was_space = false;
    for (size_t i = start; i < end; i++){
        void* current_el_ptr = get_elem_arr(char_arr, i);
        bool current_is_space = char_arr->el_type->is_separator(current_el_ptr);

        if (!current_is_space){
            arr_add_el(res_arr, res_arr->curr_size, current_el_ptr);
            last_was_space = false;
        }
        else if (!last_was_space){
            arr_add_el(res_arr, res_arr->curr_size, current_el_ptr);
            last_was_space = true;
        }
    }

    return res_arr;
}

DynamicArray* str_to_upper(const DynamicArray* char_arr){
    return str_change_case(char_arr, 1);
}

DynamicArray* str_to_lower(const DynamicArray* char_arr){
    return str_change_case(char_arr, 0);
}

void str_print(const DynamicArray* arr){
    if (!arr){
        printf("NULL\n");
        return;
    }

    if (strcmp(arr->el_type->type_name, "char") == 0){
        for (size_t i = 0; i < arr->curr_size; i++){
            arr->el_type->print(get_elem_arr(arr, i));
        }
    }
    else if(strcmp(arr->el_type->type_name, "string") == 0){
        printf("[");
        for(size_t i = 0; i < arr->curr_size; i++){
            printf("\"");
            arr->el_type->print(get_elem_arr(arr, i));
            printf("\"");
            if (i < arr->curr_size - 1){
                printf(", ");
            }
        }
        printf("]");
    }
    else{
        printf("Unknown collection type (%s)\n", arr->el_type->type_name);
    }

    printf("\n");
}