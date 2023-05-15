# the operability has been tested only on ARM-based macOS and Linux Ubuntu systems
include Makefile.autoconfig
include Makefile.misc
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
WFLAGS := -Wall \
          -Werror \
					-Wextra \
					-Wconversion \
					-Wshadow \
					-pedantic \
					-pedantic-errors
STDFLAG := -std=c99
CFLAGS :=  $(STDFLAG) -I$(HDR_DIR) $(WFLAGS)
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
export


EXE_FILE := $(EXE_DIR)/$(LAB)
INPUT_REDIRECTION := < in.txt
all: static n t

normal: n
warnings: w
optimised: o
fast: f
debug: d

n: CFLAGS := -DNDEBUG -O0 -I$(HDR_DIR)
w: CFLAGS := -DNDEBUG -O0 $(CFLAGS)
o: CFLAGS := -DNDEBUG -O2 $(CFLAGS)
f: CFLAGS := -DNDEBUG -O3 $(CFLAGS)
d: CFLAGS := -D DEBUG -g  $(CFLAGS)
n w o f d: build_dir
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(EXE_FILE)
.PHONY: n normal
.PHONY: w warnings
.PHONY: o optimised
.PHONY: f fast
.PHONY: d debug
build_dir:
	mkdir -p build
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

asan: clear
	$(CC) -g $(CFLAGS) $(ASANFLAGS) $(SRC_FILES) && $(EXE_FILE)
.PHONY: asan

cmake: cleanall
	cmake -B $(BLD_DIR)
	cmake --build $(BLD_DIR)
ctest: cmake
	cd $(BLD_DIR) && make test
.PHONY: cmake ctest

clean:
	$(RM) $(EXE_FILE)
	$(RM) $(EXE_FILE)*.dSYM
	$(RM) in.txt
	$(RM) out.txt
cleanall: clean
	$(RM) $(BLD_DIR)
.PHONY: clean cleanall
