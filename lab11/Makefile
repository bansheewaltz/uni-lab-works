# the operability has been tested only on ARM-based macOS, Linux Ubuntu systems
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
LAB := $(shell pwd | grep -o 'lab[0-9]\{1,2\}')
# project paths
SRC_DIR := src
HDR_DIR := $(SRC_DIR)/headers# private
BLD_DIR := build
EXE_DIR := $(BLD_DIR)
TEST_DIR := test
# project SRC_FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# compile options
CC := gcc
WFLAGS := -Wall -Werror -Wextra -pedantic -pedantic-errors
STDFLAG := -std=c99
CFLAGS :=  $(STDFLAG) $(WFLAGS) -I$(HDR_DIR)
ASANFLAGS := -fsanitize=address \
			 -fsanitize=leak \
			 -fsanitize=undefined
VALGRINDFLAGS := --leak-check=full \
				 --show-leak-kinds=all \
				 --track-origins=yes \
				 --trace-children=yes
CPPCHECKFLAGS := --verbose \
				 --language=c \
				 -$(STDFLAG) \
				 --enable=all \
				 --inline-suppr \
				 --suppress=missingIncludeSystem \
				 --error-exitcode=1 \
				 -DMAIN
# macros
RM := rm -rf
CLEAR_SCREEN := printf "\033c"
export


EXE_FILE := $(EXE_DIR)/$(LAB)
INPUT_REDIRECTION := < in.txt
all: static n

normal: n
warnings: w
optimised: o
fast: f
debug: d

n: CFLAGS := -DNDEBUG -I$(HDR_DIR)
w: CFLAGS := -DNDEBUG $(CFLAGS)
o: CFLAGS := -DNDEBUG -O2 $(CFLAGS)
f: CFLAGS := -DNDEBUG -O3 $(CFLAGS)
d: CFLAGS := -DDEBUG -g $(CFLAGS)
n w o f d: build_dir
	gcc $(CFLAGS) $(SRC_FILES) -o $(EXE_FILE)
build_dir:
	mkdir -p build

r: run
run:
	$(EXE_FILE)

s: static
static: clear
	cppcheck $(CPPCHECKFLAGS) -I$(HDR_DIR) $(SRC_FILES)

.PHONY: t test
t: test
test:
	$(TEST_DIR)/test$(LAB)_$(OS_SUFFIX) $(EXE_FILE)

container: cleanall
	bash $(TEST_DIR)/run_docker_image.sh

v: valgrind
valgrind: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE)
valred: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE) $(INPUT_REDIRECTION)

asan: clear
	$(CC) -g $(CFLAGS) $(ASANFLAGS) $(SRC_FILES) && $(EXE_FILE)

cmake: cleanall
	cmake -B $(BLD_DIR)
	cmake --build $(BLD_DIR)

clean:
	$(RM) $(EXE_FILE)
	$(RM) $(EXE_FILE)*.dSYM
	$(RM) in.txt
	$(RM) out.txt
cleanall: clean
	$(RM) $(BLD_DIR)

cs: clear
clear:
	$(CLEAR_SCREEN)

info:
	@echo $(LAB)
