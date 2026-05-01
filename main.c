#include "field_info.h"
#include "types.h"
#include "dynamic_array.h"
#include "string_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

static DynamicArray* str_read_from_stdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    DynamicArray* str = arr_create(get_char_info(), 8);
    if (!str){
        fprintf(stderr, "str_read_from_stdin error (arr_create return NULL)\n");
        return NULL;
    }

    int symbol;
    while (true){
        symbol = getchar();
        if (symbol == '\n' || symbol == EOF){
            break;
        }
        char value = (char)symbol;
        arr_add_el(str, str->curr_size, &value);
    }

    return str;
}

static void print_menu() {
    printf("\n==== Управление коллекцией ====\n");
    printf("1) Ввести новую строку (создать char-коллекцию)\n");
    printf("2) Очистить лишние пробелы\n");
    printf("3) Разбить на слова\n");
    printf("4) Перевести в ВЕРХНИЙ регистр\n");
    printf("5) Перевести в нижний регистр\n");
    printf("6) Склеить слова в строку символов\n");
    printf("7) Взять подстроку\n");
    printf("8) Вывести текущие данные\n");
    printf("0) Выход\n");
    printf("Выберите действие: ");
}

int main(){
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int command = -1;
    DynamicArray* data = NULL;

    while (command != 0){
        print_menu();
        if (scanf("%d", &command) != 1){
            printf("Введен некорректный номер\n");
            continue;
        }

        switch(command){
            case 1:
                printf("Введите строку: ");
                data = str_read_from_stdin();
                if (!data){
                    arr_destroy(data);
                    printf("Не получилось создать коллекцию\n");
                    break;
                }
                printf("\nКоллекция создана\n");
                break;
            
            case 2:
                if (!data){
                    printf("Сначала создайте коллекцию\n");
                    break;
                }

                DynamicArray* trimmed = str_trim_spaces(data);
                if (!trimmed){
                    printf("Ошибка при удалении пробелов\n");
                    break;
                }
                arr_destroy(data);
                data = trimmed;
                printf("Лишние пробелы удалены\n");
                break;
            
            case 3:
                if (!data){
                    printf("Сначала создайте коллекцию\n");
                    break;
                }

                if (!is_types_equal(data->el_type, get_char_info())){
                    printf("Разделение доступно только для коллекции символов\n");
                    break;
                }
                DynamicArray* splitted = str_split_to_words(data);
                if (!splitted){
                    printf("Ошибка при разбиении строки\n");
                    break;
                }
                arr_destroy(data);
                data = splitted;
                printf("Строка разбита на слова\n");
                break;
            
            case 4:
                if (!data){
                    printf("Сначала создайте коллекцию\n");
                    break;
                }

                DynamicArray* upped = str_to_upper(data);
                if (!upped){
                    printf("Ошибка при изменении регистра на высокий\n");
                    break;
                }
                arr_destroy(data);
                data = upped;
                printf("Все символы теперь в ВЕРХНЕМ регистре\n");
                break;
                
            case 5:
                if (!data){
                    printf("Сначала создайте коллекцию\n");
                    break;
                }

                DynamicArray* lowered = str_to_lower(data);
                if (!lowered){
                    printf("Ошибка при изменении регистра на низкий\n");
                    break;
                }
                arr_destroy(data);
                data = lowered;
                printf("Все слова теперь в нижнем регистре\n");
                break;

            case 6:
                if (!data){
                    printf("Сначала создайте коллекцию\n");
                    break;
                }

                if (!is_types_equal(data->el_type, get_string_info())) {
                    printf("Склеивание доступно только для коллекции строк\n");
                    break;
                }
                DynamicArray* chared = str_to_char(data);
                if (!chared){
                    printf("Ошибка при склеивании слов\n");
                    break;
                }
                arr_destroy(data);
                data = chared;
                printf("Слова склеены в строку");
                break;

            case 7:
                if (!data){
                    printf("Сначала создайте коллекцию\n");
                    break;
                }

                size_t start;
                size_t end;

                printf("Введите индексы start и end: ");
                scanf("%zu %zu", &start, &end);
                printf("\n");

                DynamicArray* subbed = str_sub_range(data, start, end);
                if (!subbed){
                    printf("Ошибка при взятии подстроки\n");
                    break;
                }
                arr_destroy(data);
                data = subbed;
                break;

            case 8:
                if (!data){
                    printf("Данные для вывода отсутствуют\n");
                }
                else{
                    printf("Текущий данные (тип %s): ", data->el_type->type_name);
                    str_print(data);
                }
                break;

            case 0:
                printf("Завершение программы\n");
                break;

            default:
                printf("Введен неверный номер команды\n");
        }
    }

    if (data){
        arr_destroy(data);
    }

    return 0;
}