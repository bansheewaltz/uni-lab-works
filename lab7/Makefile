# has been tested on ARM-based macOS and Linux Ubuntu
include Makefile.autoconfig
include Makefile.misc
# project paths
SRC_DIR := src
HDR_DIR := $(SRC_DIR)/headers# private headers
BLD_DIR := build
EXE_DIR := $(BLD_DIR)
TEST_DIR := test
# project SRC_FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# compile options
CC := gcc
WFLAGS := -Wall \
          -Werror \
					-Wextra \
					-Wconversion \
					-Wshadow \
					-pedantic \
					-pedantic-errors
STDFLAG := -std=c99
CFLAGS :=  $(STDFLAG) -I$(HDR_DIR) $(WFLAGS)
SANFLAGS := -fsanitize=address \
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


EXE_FILE := $(EXE_DIR)/$(LAB)
INPUT_REDIRECTION := < in.txt
all: static n t

normal: n
warnings: w
debug: d
optimised: o
fast: f

n: CFLAGS := -DNDEBUG -O0 -I$(HDR_DIR)
w: CFLAGS := -DNDEBUG -O0 $(CFLAGS)
d: CFLAGS := -D DEBUG -g  $(CFLAGS)
o: CFLAGS := -DNDEBUG -O2 -I$(HDR_DIR)
f: CFLAGS := -DNDEBUG -O3 -I$(HDR_DIR)
n w o f d: build_dir
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(EXE_FILE)
.PHONY: n normal
.PHONY: w warnings
.PHONY: o optimised
.PHONY: f fast
.PHONY: d debug
build_dir:
	$(MK) $(BLD_DIR)
.PHONY: build_dir

r: run
run:
	$(EXE_FILE)
.PHONY: r run

s: static
static: clear
	cppcheck $(CPPCHECKFLAGS) -I$(HDR_DIR) $(SRC_FILES)
.PHONY: s static

t: test
test:
	$(TEST_DIR)/test$(LAB)_$(PLATFORM) $(EXE_FILE)
.PHONY: t test

container: cleanall
	bash $(TEST_DIR)/run_docker_image.sh
.PHONY: container

v: valgrind
valgrind: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE)
valred: clear d
	valgrind $(VALGRINDFLAGS) $(EXE_FILE) $(INPUT_REDIRECTION)
.PHONY: v valgring valred

san: clear
	$(CC) -g $(CFLAGS) $(SANFLAGS) $(SRC_FILES) && $(EXE_FILE)
.PHONY: san

cmake: cleanall
	cmake -B $(BLD_DIR)
	cmake --build $(BLD_DIR)
ctest: cmake
	cd $(BLD_DIR) && make test
ccheck: cmake
	cd $(BLD_DIR) && make check
.PHONY: cmake ctest ccheck

tidy:
	clang-tidy $(shell find . -type f -name '*.c') -- -I$(HDR_DIR)

clean:
	$(RM) $(BLD_DIR)
cleanmisc:
	$(RM) in.txt
	$(RM) out.txt
	$(RM) a.out
cleanall: clean cleanmisc
.PHONY: clean cleanmisc cleanall
