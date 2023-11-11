IN=src/main.c
TEST_IN=src/tests.c
OUT=bin/main
TEST_OUT=bin/tests.exe

CC=gcc -Wall

build:
	$(CC) -c $(IN) -o $(OUT).o
	$(CC) $(OUT).o -o $(OUT) -lregx -L ./bin


l: lib/match.c lib/parser.c lib/regex.h
	$(CC) -o bin/libregx.so -fpic -shared lib/match.c lib/parser.c 

run: build
	LD_LIBRARY_PATH="./bin/" $(OUT)
r: run

test:
	$(CC) $(TEST_IN) -o $(TEST_OUT)
	$(TEST_OUT)
t: test

clean:
	rm -rf ./bin/*
