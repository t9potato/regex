IN=src/main.c
TEST_IN=src/tests.c
OUT=bin/main.exe
TEST_OUT=bin/tests.exe

CC=gcc -Wall

build:
        $(CC) $(IN) -o $(OUT)

lib:
	$(CC)

l: lib

run: build
        $(OUT)
r: run

test:
        $(CC) $(TEST_IN) -o $(TEST_OUT)
        $(TEST_OUT)
t: test
