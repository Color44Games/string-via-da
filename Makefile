OBJ_APP = main.o dynamic_array.o string_collection.o types.o
OBJ_TEST = tests.o dynamic_array.o string_collection.o types.o

ifeq ($(OS), Windows_NT)
	DEL_COM = del /Q *.o *.exe
else
	DEL_COM = rm -f *.o app test
endif

app: $(OBJ_APP)
	gcc -g -o app $^

test: $(OBJ_TEST)
	gcc -g -o test $^

tests.o:
	gcc tests.c -c

main.o: main.c
	gcc main.c -c

dynamic_array.o: dynamic_array.c dynamic_array.h
	gcc dynamic_array.c -c

string_collection.o: string_collection.c string_collection.h
	gcc string_collection.c -c

types.o: types.c types.h field_info.h
	gcc types.c -c

.PHONY: clean
clean:
	$(DEL_COM)

