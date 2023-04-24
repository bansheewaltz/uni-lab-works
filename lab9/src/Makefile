# utility stuff
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	OS := macOS
endif
LAB := $(shell pwd | grep -o 'lab[0-9]')
# compile options
CC := gcc
WFLAGS := -Wall -Werror -Wextra
STDFLAG := -std=c11
CFLAGS := $(WFLAGS) $(STDFLAG)
MEMFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes
ASANFLAGS := -fsanitize=address -fsanitize=leak -fsanitize=undefined
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
	./test$(LAB) $(EXE)
static: clear
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
v: valgrind
valgrind: clear d
	valgrind $(MEMFLAGS) ./$(EXE)
valred: clear d
	valgrind $(MEMFLAGS) ./$(EXE) $(INPUT_REDIRECTION)
asan: clear
	$(CC) -g $(CFLAGS) $(ASANFLAGS) $(FILES) && ./$(EXE)
clean:
	$(RM) $(EXE)
	$(RM) *.out
	$(RM) *.dSYM
clear:
	clear
container: clean
	bash test/run_docker_image.sh