#include "types.h"
#include "string_collection.h"
#include "dynamic_array.h"
#include "field_info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#endif

static int passed = 0;
static int failed = 0;

typedef int (*test_func_ptr)();

static int verify(const DynamicArray* arr, const char* expected) {
    if (!arr){
        return 0;
    }
    char* actual = str_to_cstring(arr);
    printf("%s", actual);
    int res = (strcmp(actual, expected) == 0);
    if (!res) {
        printf("\n[FAIL] Ожидалось: '%s', Получено: '%s'\n", expected, actual);
    }
    free(actual);
    return res;
}

static void check(test_func_ptr test, int* passed, int* failed){
    if (test){ 
        printf("PASSED\n"); 
        (*passed)++; 
    } 
    else{
        printf("FAILED\n");
        (*failed)++;
    }
}

int test_field_info_copy(){
    const FieldInfo* char_test = get_char_info();
    const FieldInfo* string_test = get_string_info();

    const char source_char = 'A';
    const char* source_str = "Abc";

    char destination_char = 'Z';
    char* destination_str = "Zyx";

    char_test->copy(&destination_char, &source_char);
    string_test->copy(&destination_str, &source_str);

    int ok1 = (strcmp(destination_str, source_str) == 0);
    int ok2 = (destination_char == source_char);

    return (ok1 && ok2);
}

int test_field_info_free_elem(){
    const FieldInfo* char_test = get_char_info();
    const FieldInfo* string_test = get_string_info();
    
    char* str_elem = "ToS";

    string_test->free_elem(&str_elem);

    int ok1 = (char_test->free_elem == NULL);
    int ok2 = ((&str_elem) == NULL);

    return ok1 && ok2;
}

int test_field_info_compare(){
    const FieldInfo* char_test = get_char_info();
    const FieldInfo* string_test = get_string_info();

    char char_first_elem = 'A';
    char char_second_elem = 'b';
    char char_third_elem = 'A';

    char* str_first_elem = "ABC";
    char* str_second_elem = "123";
    char* str_third_elem = "abc";

    int compare1 = char_test->compare(&char_first_elem, &char_second_elem);
    int compare2 = char_test->compare(&char_first_elem, &char_third_elem);
    int compare3 = char_test->compare(&char_second_elem, &char_third_elem);

    int compare4 = string_test->compare(&str_first_elem, &str_second_elem);
    int compare5 = string_test->compare(&str_first_elem, &str_third_elem);
    int compare6 = string_test->compare(&str_second_elem, &str_third_elem);

    int ok1 = (compare1 < 0);
    int ok2 = (compare2 == 0);
    int ok3 = (compare3 > 0);
    int ok4 = (compare4 < 0);
    int ok5 = (compare5 < 0);
    int ok6 = (compare6 < 0);

    return ok1 && ok2 && ok3 && ok4 && ok5 && ok6;
}

int test_field_info_print() {
    const FieldInfo* char_test = get_char_info();
    const FieldInfo* string_test = get_string_info();

    char char_elem = 'X';
    char* str_elem = "Hello"; 
    char buffer[256] = {0};
    
    #ifdef _WIN32
        freopen("nul", "a", stdout);  
    #else
        freopen("/dev/null", "a", stdout);
    #endif
    
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));
    
    char_test->print(&char_elem);
    int ok1 = (buffer[0] == 'X');
    
    memset(buffer, 0, sizeof(buffer));
    
    string_test->print(&str_elem);
    int ok2 = (strcmp(buffer, "Hello") == 0);
    
    #ifdef _WIN32
        freopen("CON", "w", stdout);
    #else
        freopen("/dev/tty", "w", stdout);
    #endif

    return ok1 && ok2;
}

int test_field_info_is_separator() {
    const FieldInfo* char_test = get_char_info();
    const FieldInfo* string_test = get_string_info();

    char space = ' ';
    char newline = '\n';
    char tab = '\t';
    char letter = 'H';
    char digit = '5';

    int ok1 = char_test->is_separator(&space);
    int ok2 = char_test->is_separator(&newline);
    int ok3 = char_test->is_separator(&tab);
    int ok4 = !char_test->is_separator(&letter);
    int ok5 = !char_test->is_separator(&digit);

    return ok1 && ok2 && ok3 && ok4 && ok5;
}

int test_trim() {
    // Обычный пример
    DynamicArray* s1 = str_create_from_cstring("  hello world  ");
    DynamicArray* t1 = str_trim_spaces(s1);
    int ok1 = verify(t1, "hello world");
    
    // Строка только из пробелов
    DynamicArray* s2 = str_create_from_cstring("   ");
    DynamicArray* t2 = str_trim_spaces(s2);
    int ok2 = verify(t2, "");
    
    arr_destroy(s1); 
    arr_destroy(t1);
    arr_destroy(s2); 
    arr_destroy(t2);
    return (ok1 && ok2);
}

int test_range() {
    DynamicArray* s = str_create_from_cstring("012345");
    
    // Обычный пример
    DynamicArray* r1 = str_sub_range(s, 1, 4);
    int ok1 = verify(r1, "123");
    
    // Конец больше размера массива
    DynamicArray* r2 = str_sub_range(s, 3, 100);
    int ok2 = verify(r2, "345");
    
    // Начало больше конца
    DynamicArray* r3 = str_sub_range(s, 5, 2);
    int ok3 = (r3->curr_size == 0);
    
    arr_destroy(s); 
    arr_destroy(r1); 
    arr_destroy(r2); 
    arr_destroy(r3);
    return (ok1 && ok2 && ok3);
}

int test_concat() {
    DynamicArray* a = str_create_from_cstring("Hello");
    DynamicArray* b = str_create_from_cstring("World");
    DynamicArray* res = str_concat(a, b);
    
    int ok = verify(res, "HelloWorld");
    
    arr_destroy(a); 
    arr_destroy(b); 
    arr_destroy(res);
    return ok;
}

int test_several_command() {
    DynamicArray* s1 = str_create_from_cstring("Hello");
    DynamicArray* s2 = str_create_from_cstring("World");
    
    // Склеиваем, переводим в верхний регистр, потом берем подстроку
    DynamicArray* c1 = str_concat(s1, s2);       // "HelloWorld"
    DynamicArray* u1 = str_to_upper(c1);         // "HELLOWORLD"
    DynamicArray* r1 = str_sub_range(u1, 0, 5);  // "HELLO"
    
    int ok = verify(r1, "HELLO");

    arr_destroy(s1); 
    arr_destroy(s2); 
    arr_destroy(c1); 
    arr_destroy(u1);
    arr_destroy(r1);
    return ok;
}

int test_cases() {
    int ok = 1;

    // Смешанный текст с цифрами
    DynamicArray* s1 = str_create_from_cstring("123 Hello-World!");
    DynamicArray* up = str_to_upper(s1);
    DynamicArray* low = str_to_lower(s1);

    int ok1 = verify(up, "123 HELLO-WORLD!");
    int ok2 = verify(low, "123 hello-world!");

    // Пустая строка
    DynamicArray* s2 = str_create_from_cstring("");
    DynamicArray* up_empty = str_to_upper(s2);
    int ok3 = (up_empty->curr_size == 0);

    arr_destroy(s1); 
    arr_destroy(up); 
    arr_destroy(low);
    arr_destroy(s2); 
    arr_destroy(up_empty);
    return (ok1 && ok2 && ok3);
}

int main() {
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    printf("==== ЗАПУСК ТЕСТОВ ====\n");

    check(test_field_info_copy, &passed, &failed);
    check(test_field_info_free_elem, &passed, &failed);
    check(test_field_info_compare, &passed, &failed);
    check(test_field_info_print, &passed, &failed);
    check(test_field_info_is_separator, &passed, &failed);
    check(test_trim, &passed, &failed);
    check(test_range, &passed, &failed);
    check(test_concat, &passed, &failed);
    check(test_several_command, &passed, &failed);
    check(test_cases, &passed, &failed);

    printf("==== ИТОГ: Пройдено %d из %d тестов ====\n", passed, failed + passed);
    
    return (passed == (passed + failed)) ? 0 : 1;
}