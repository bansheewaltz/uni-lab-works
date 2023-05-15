#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

void print_error(char *file, int line, char *error_msg) {
  fprintf(stderr, "Internal Error: %s:%d: %s\n", file, line, error_msg);
}

void *safe_malloc(size_t size, char *file, int line) {
  void *pointer = malloc(size);
  if (pointer == NULL) {
    print_error(file, line, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return pointer;
}

void *safe_calloc(size_t n, size_t size, char *file, int line) {
  void *pointer = calloc(n, size);
  if (pointer == NULL) {
    print_error(file, line, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return pointer;
}

void stack_array_print(int *stack_array, int length) {
  for (int i = length; i >= 0; --i) {
    if (stack_array[i] != 0) {
      printf("%d%s", stack_array[i], i == 0 ? "" : " ");
    }
  }
}
