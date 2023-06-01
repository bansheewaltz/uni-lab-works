# has been tested on ARM-based macOS and Linux Ubuntu
include Makefile.autoconfig
# project paths
SRC_DIR := src
HDR_DIR := $(SRC_DIR)/headers# private headers
BLD_DIR := build
EXE_DIR := $(BLD_DIR)
TEST_DIR := test
TEST_SCRIPTS_DIR     := $(TEST_DIR)/scripts
TEST_BINARIES_DIR    := $(TEST_DIR)/binaries
TEST_DOCKERFILES_DIR := $(TEST_DIR)/Dockerfiles
# project SRC_FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# compile options
CC := gcc
STD ?= c99
STDFLAG := -std=$(STD)
CFLAGS = -I$(HDR_DIR) $(STDFLAG)
LDFLAGS := -lm

WFLAGS :=        -Wall \
                 -Werror \
                 -Wextra \
                 -Wconversion \
                 -Wshadow \
                 -pedantic \
                 -pedantic-errors

SANFLAGS :=      -fsanitize=address \
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
export

EXE_FILE := $(EXE_DIR)/$(EXE_NAME)
INPUT_REDIRECTION := < in.txt


#==============================================================================
### Standard targets:

all: n t

normal: n
warnings: w
debug: d
optimised: o
fast: f

w: CFLAGS := -DNDEBUG -O0 $(CFLAGS) $(WFLAGS)
n: CFLAGS := -DNDEBUG -O0 $(CFLAGS)
d: CFLAGS := -D DEBUG -g2 $(CFLAGS)
o: CFLAGS := -DNDEBUG -O2 $(CFLAGS)
f: CFLAGS := -DNDEBUG -O3 $(CFLAGS)
n w o f d: build_dir
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(EXE_FILE) $(LDFLAGS)
.PHONY: n normal w warnings o optimised f fast d debug

build_dir:
	$(MK) $(BLD_DIR)
.PHONY: build_dir

r: run
run:
	$(EXE_FILE)
.PHONY: r run

t: test
test:
	$(TEST_BINARIES_DIR)/test$(LAB)_$(PLATFORM) $(EXE_FILE)
.PHONY: t test


#==============================================================================
### Cmake target callers:

cmake: clean
	cmake -B $(BLD_DIR)
	cmake --build $(BLD_DIR)
ctest: cmake
	cd $(BLD_DIR) && make test
ccheck: cmake
	cd $(BLD_DIR) && make check
.PHONY: cmake ctest ccheck


#==============================================================================
### Gitlab test_runner copycat:

runner:
	image=native platform=linux/amd64 $(MAKE) container
.PHONY: runner


#==============================================================================
### Custom checks:

in:
	$(EXE_FILE) $(TEST_DIR)/in_normal.txt $(TEST_DIR)/encoded_out.txt
.PHONY: in

out:
	$(EXE_FILE) $(TEST_DIR)/in_encoded.txt $(TEST_DIR)/decoded_out.txt
.PHONY: out

c: cppcheck
cppcheck: clear
	cppcheck $(CPPCHECKFLAGS) -I$(HDR_DIR) $(SRC_FILES)
.PHONY: s cppcheck

v: valgrind
valgrind: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE)
valred: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE) $(INPUT_REDIRECTION)
.PHONY: v valgring valred

san: clear build_dir
	$(CC) -g $(CFLAGS) $(SANFLAGS) $(SRC_FILES) $(LDFLAGS) -o $(EXE_FILE)
.PHONY: san

tidy:
	clang-tidy $(call find_dot_c_h) -- -I$(HDR_DIR) $(STDFLAG)
.PHONY: tidy

linter_fix: LINTFLAGS := -i
linter_check: LINTFLAGS := -n
linter_check linter_fix: clean
	clang-format $(LINTFLAGS) -style=file $(call find_dot_c_h)
.PHONY: linter_check linter_fix

find_dot_c_h = $(shell find . -type f -name '*.h' -o -name '*.c')


#==============================================================================
### Docker:

container: clean
	bash $(TEST_SCRIPTS_DIR)/run_docker_image.sh
.PHONY: container


#==============================================================================
### Cleaning routines:

clean:
	$(RM) $(BLD_DIR)
cleanbuild:
	cd $(BLD_DIR) && ls | grep -xv "$(EXE_NAME)" | xargs $(RM)
cleanmisc:
	$(RM) in.txt
	$(RM) out.txt
	$(RM) a.out
	$(RM) tmp
	$(RM) *.dSYM
cleanall: clean cleanmisc
.PHONY: clean cleanbuild cleanmisc cleanall

cs: clear
clear:
	$(CLEAR_SCREEN)
.PHONY: cs clear


#==============================================================================
### Misc:

info:
	@echo $(PLATFORM)
.PHONY: info
