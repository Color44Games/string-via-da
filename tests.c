#include "types.h"
#include "string_collection.h"
#include "dynamic_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static int verify(const DynamicArray* arr, const char* expected) {
    if (!arr){
        return 0;
    }
    char* actual = str_to_cstring(arr);
    int res = (strcmp(actual, expected) == 0);
    if (!res) {
        printf("\n[FAIL] Ожидалось: '%s', Получено: '%s'\n", expected, actual);
    }
    free(actual);
    return res;
}

int test_trim() {
    printf("Тест Space_trim: ");
    
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
    printf("Тест Sub_range: ");
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
    printf("Тест Concat: ");
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
    printf("Тест несколько команд: ");
    
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
    printf("Тест Upper/Lower: ");
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
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int passed = 0;
    int total = 4;

    printf("==== ЗАПУСК ТЕСТОВ ====\n");

    if (test_trim())           { printf("PASSED\n"); passed++; } else printf("FAILED\n");
    if (test_range())          { printf("PASSED\n"); passed++; } else printf("FAILED\n");
    if (test_concat())         { printf("PASSED\n"); passed++; } else printf("FAILED\n");
    if (test_cases())          { printf("PASSED\n"); passed++; } else printf("FAILED\n");

    printf("\n==== ИТОГ: Пройдено %d из %d тестов ====\n", passed, total);
    
    return (passed == total) ? 0 : 1;
}