GCC = gcc
CFLAGS = -c -Wall -Wextra -Werror -std=c11 -pedantic

all: build

build: s21_cat

s21_cat: s21_cat.o
	$(GCC) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c s21_cat.h
	$(GCC) $(CFLAGS) s21_cat.c -o s21_cat.o

clean:
	rm -f *.o
	rm -f s21_cat
	clear

rebuild: clean build

test_style: cpp cl

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem --std=c11 *.c

cl:
	clang-format -n *.c
	clang-format -n *.h

val1:
	valgrind --tool=memcheck --leak-check=yes ./s21_cat $(opt) tests/tests_files/char.txt

val2:
	valgrind --tool=memcheck --leak-check=yes ./s21_cat $(opt) tests/tests_files/test_1_cat.txt

val3:
	valgrind --tool=memcheck --leak-check=yes ./s21_cat $(opt) tests/tests_files/test_case_cat.txt


test:
	tests/tests_cat.sh

