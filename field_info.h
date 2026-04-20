#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stdbool.h>
#include <stddef.h>

typedef struct FieldInfo {
    size_t elem_size;
    void (*copy) (const void* destination, const void* sourse);
    void (*free_elem) (const void* elem);
    int (*compare) (const void* first_elem, const void* second_elem);
    void (*print) (const void* elem);
    bool (*is_separator) (const void* elem);
    const char* type_name;
} FieldInfo;

static inline bool is_types_equal(const FieldInfo* a, const FieldInfo* b){
    return a == b;
}

#endif