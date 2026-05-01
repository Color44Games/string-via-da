#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dynamic_array.h"

#define PTR_ELEM(arr, index) ((arr)->data + (index) * (arr)->el_type->elem_size)

#define DEFAULT_START_CAPACITY 8
#define INCREASE_COEFFICIENT 2

static bool arr_resize(DynamicArray* arr, const size_t need){
    if (!arr){
        return false;
    }

    if (need <= arr->capacity){
        return true;
    }

    size_t new_capacity = arr->capacity;
    while(new_capacity < need){
        new_capacity *= INCREASE_COEFFICIENT;
    }

    void* new_data = realloc(arr->data, new_capacity * arr->el_type->elem_size);
    if (!new_data){
        fprintf(stderr, "arr_resize error (realloc failure)\n");
        return false;
    }

    arr->capacity = new_capacity;
    arr->data = new_data;

    return true;
}

DynamicArray* arr_create(const FieldInfo* type, size_t start_capacity){
    if (!type){
        fprintf(stderr, "arr_create error (NULL type)\n");
        return NULL;
    }   

    if (start_capacity <= 0){
        fprintf(stderr, "arr_create warning (start_capacity must be positive)\nSet default start_capacity = %d\n", DEFAULT_START_CAPACITY);
        start_capacity = DEFAULT_START_CAPACITY;
    }

    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!arr){
        fprintf(stderr, "arr_create error (malloc failure)\n");
        return NULL;
    }

    arr->data = (void*)malloc(start_capacity * type->elem_size);
    if (!arr->data){
        free(arr);
        fprintf(stderr, "arr_create error (malloc of data failure)\n");
        return NULL;
    }

    arr->capacity = start_capacity;
    arr->curr_size = 0;
    arr->el_type = type;

    return arr;
}

void arr_destroy(DynamicArray* arr){
    if (!arr){
        return;
    }

    if (arr->el_type->free_elem){
        for(size_t i = 0; i < arr->curr_size; i++){
            arr->el_type->free_elem(PTR_ELEM(arr, i));
        }
    }

    free(arr->data);
    free(arr);
}

void* get_elem_arr(const DynamicArray* arr, const size_t index){
    if (!arr){
        fprintf(stderr, "get_elem arr error (arr is NULL)\n");
        return NULL;
    }
    
    if (index >= arr->curr_size){
        fprintf(stderr, "get_elem_arr error (index is larger than arr size)\n Size now = %zu\n", arr->curr_size);
        return NULL;
    }

    return PTR_ELEM(arr, index);
}

bool set_elem_arr(DynamicArray* arr, const size_t index, const void* value){
    if (!arr || !value){
        fprintf(stderr, "set_elem_arr error (received NULL pointer)\n");
        return false;
    }
    if (index >= arr->curr_size){
        fprintf(stderr, "set_elem_arr error (index is larger than arr size)\n Size now = %zu\n", arr->curr_size);
        return false;
    }

    void* dest = PTR_ELEM(arr, index);
    if (arr->el_type->free_elem){
        arr->el_type->free_elem(dest);
    }
    arr->el_type->copy(dest, value);

    return true;
}

bool arr_add_el(DynamicArray* arr, const size_t index, const void* value){
    if (!arr || !value){
        fprintf(stderr, "arr_add_el error (received NULL pointer)\n");
        return false;
    }
    if (index > arr->curr_size){
        fprintf(stderr, "arr_add_el error (index is larger than arr size)\n Size now = %zu\n", arr->curr_size);
        return false;
    }

    if (!arr_resize(arr, arr->curr_size + 1)){
        return false;
    }

    memmove(PTR_ELEM(arr, index + 1), PTR_ELEM(arr, index), (arr->curr_size - index) * arr->el_type->elem_size);
    arr->el_type->copy(PTR_ELEM(arr, index), value);
    arr->curr_size++;
    
    return true;
}

bool arr_remove_el(DynamicArray* arr, const size_t index){
    if (!arr){
        fprintf(stderr, "arr_remove_el error (arr is NULL)\n");
        return false;
    }

    if (index >= arr->curr_size){
        fprintf(stderr, "arr_remove_el error (index is larger than arr size)\n Size now = %zu\n", arr->curr_size);
        return false;
    }

    if (arr->el_type->free_elem){
        arr->el_type->free_elem(PTR_ELEM(arr, index));
    }
    
    if (index < arr->curr_size - 1){
        memmove(PTR_ELEM(arr, index), PTR_ELEM(arr, index + 1), (arr->curr_size - index - 1) * arr->el_type->elem_size);
    }
    arr->curr_size--;

    return true;
}