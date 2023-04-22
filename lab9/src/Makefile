# utility stuff
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	OS := macOS
endif
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
all: static
	gcc -g $(CFLAGS) $(FILES)
d: debug
debug:
	$(CC) -g -DDEBUG $(CFLAGS) $(FILES)
static: clear
	cppcheck --enable=all --suppress=missingIncludeSystem *.[ch]
valgrind: clear d
	valgrind $(MEMFLAGS) ./$(EXE)
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