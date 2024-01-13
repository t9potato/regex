IN=tests/main.c
OUT=bin/main
CC=gcc -Wall -g
ifeq ($(OS), Windows_NT)
	LIB_TARGET = regx.dll
else
	LIB_TARGET = libregx.so
endif

build: l
	$(CC) -c $(IN) -o $(OUT).o
	$(CC) $(OUT).o -o $(OUT) -lregx -L ./bin


l: lib/match.c lib/parser.c lib/regex.h
	$(CC) -o bin/$(LIB_TARGET) -fpic -shared lib/match.c lib/parser.c 

test: build
	LD_LIBRARY_PATH="./bin/" $(OUT)
tv: build
	LD_LIBRARY_PATH="./bin/" valgrind --leak-check=yes $(OUT)

clean:
	rm -rf ./bin/*
