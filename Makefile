app: main.o dynamic_array.o string_collection.o types.o
	gcc -g main.o dynamic_array.o string_collection.o types.o -o app

test: tests.o dynamic_array.o string_collection.o types.o
	gcc -g tests.o dynamic_array.o string_collection.o types.o -o test

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
	del /Q app.exe test.exe *.o

