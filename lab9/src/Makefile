# utility stuff
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	OS := macOS
endif
ifeq ($(UNAME_S), Linux)
	OS := Linux
endif
ifeq ($(OS), macOS)
	OS_SUFFIX := mac
endif
ifeq ($(OS), Linux)
	OS_SUFFIX := linux
endif
LAB := $(shell pwd | grep -o 'lab[0-9]')
TEST_DIR := ../test
# compile options
CC := gcc
WFLAGS := -Wall -Werror -Wextra
STDFLAG := -std=c99
CFLAGS := $(WFLAGS) $(STDFLAG)
ASANFLAGS := -fsanitize=address -fsanitize=leak -fsanitize=undefined
VALGRINDFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes
CPPCHECKFLAGS := --enable=all --suppress=missingIncludeSystem
# macro
RM := rm -rf
export


FILES := main.c
EXE := a.out
INPUT_REDIRECTION := < in.txt
all: static
	gcc -g $(CFLAGS) $(FILES)
n:
	gcc -g $(CFLAGS) $(FILES)
r: run
run:
	./$(EXE)
d: debug
debug:
	$(CC) -g -DDEBUG $(CFLAGS) $(FILES)
t: test
test: n
	$(TEST_DIR)/test$(LAB)_$(OS_SUFFIX) $(EXE)
static: clear
	cppcheck $(CPPCHECKFLAGS) *.[ch]
v: valgrind
valgrind: clear d
	valgrind $(VALGRINDFLAGS) ./$(EXE)
valred: clear d
	valgrind $(VALGRINDFLAGS) ./$(EXE) $(INPUT_REDIRECTION)
asan: clear
	$(CC) -g $(CFLAGS) $(ASANFLAGS) $(FILES) && ./$(EXE)
clean:
	$(RM) $(EXE)
	$(RM) *.out
	$(RM) *.dSYM
clear:
	clear
container: clean
	bash $(TEST_DIR)/run_docker_image.sh