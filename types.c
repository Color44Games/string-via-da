#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ==== Char type ====

static void char_copy(const void* destination, const void* source){
    *(char*) destination = *(const char*) source;
}

static int char_compare(const void* first_elem, const void* second_elem){
    const char fi = *(const char*) first_elem;
    const char se = *(const char*) second_elem;

    return (fi > se) - (fi < se);
}

static void char_print(const void* elem){
    printf("%c", *(const char*) elem);
}

static bool char_is_separator(const void* elem){
    return isspace(*(unsigned char*) elem);
}

static FieldInfo CHAR_TYPE_INFO = {
    .elem_size = sizeof(char),
    .copy = char_copy,
    .free_elem = NULL,
    .compare = char_compare,
    .print = char_print,
    .is_separator = char_is_separator,
    .type_name = "char"
};

const FieldInfo* get_char_info(void){
    return &CHAR_TYPE_INFO;
}

// ==== String type ====

static void string_copy(const void* destination, const void* source){
    *(char**) destination = *(const char**) source;
}

static void string_free_elem(const void* elem){
    free(*(char**)elem);
}

static int string_compare(const void* first_elem, const void* second_elem){
    const char* fi = *(const char**) first_elem;
    const char* se = *(const char**) second_elem;

    if (!fi || !se){
        return (fi > se) - (fi < se);
    }
    return strcmp(fi, se);
}

static void string_print(const void* elem){
    printf("%s", *(const char**) elem ? *(const char**) elem : "NULL");
}

static FieldInfo STRING_TYPE_INFO = {
    .elem_size = sizeof(char*),
    .copy = string_copy,
    .free_elem = string_free_elem,
    .compare = string_compare,
    .print = string_print,
    .is_separator = NULL,
    .type_name = "string"
};

const FieldInfo* get_string_info(void){
    return &STRING_TYPE_INFO;
}