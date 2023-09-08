GCC = gcc
CFLAGS = -c -Wall -Wextra -Werror -std=c11 -pedantic

all: build

build: s21_grep

s21_grep: s21_grep.o
	$(GCC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	$(GCC) $(CFLAGS) s21_grep.c -o s21_grep.o

clean:
	rm -f *.o
	rm -f s21_grep
	clear

rebuild: clean build

test: cpp cl

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem --std=c11 *.c

cl:
	clang-format -n *.c
	clang-format -n *.h

val:
	valgrind --tool=memcheck --leak-check=yes ./s21_grep Lisp tests/tests_files/*.txt -i -n
