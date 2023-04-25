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
# project paths
SRC_DIR := src
HDR_DIR := $(SRC_DIR)/headers# private
EXE_DIR := build
TEST_DIR := test
# project SRC_FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# compile options
CC := gcc
WFLAGS := -Wall -Werror -Wextra
STDFLAG := -std=c99
CFLAGS := $(WFLAGS) $(STDFLAG) -I$(HDR_DIR)
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
all: static
	gcc -g $(CFLAGS) $(SRC_FILES) -o $(EXE_FILE)
n:
	gcc -g $(CFLAGS) $(SRC_FILES) -o $(EXE_FILE)
r: run
run:
	$(EXE_FILE)
d: debug
debug:
	$(CC) -g -DDEBUG $(CFLAGS) $(SRC_FILES)
t: test
test: clear
	$(TEST_DIR)/test$(LAB)_$(OS_SUFFIX) $(EXE_FILE)
v: valgrind
valgrind: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE)
s: static
static: clear
	cppcheck $(CPPCHECKFLAGS) -I $(HDR_DIR) $(SRC_FILES)
valred: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE) $(INPUT_REDIRECTION)
asan: clear
	$(CC) -g $(CFLAGS) $(ASANFLAGS) $(SRC_FILES) && $(EXE_FILE)
clean:
	$(RM) $(EXE_FILE)
	$(RM) $(EXE_FILE)*.dSYM
clear:
	$(CLEAR_SCREEN)
# clear:
# 	clear
container: clean
	bash $(TEST_DIR)/run_docker_image.sh
