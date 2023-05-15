#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

void *safe_malloc(size_t size, char *file, uint line) {
  void *pointer = malloc(size);
  if (pointer == NULL) {
    fprintf(stdout, "Internal Error: %s:%d: %s\n", file, line, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return pointer;
}

void *safe_calloc(size_t n, size_t size, char *file, uint line) {
  void *pointer = calloc(n, size);
  if (pointer == NULL) {
    fprintf(stdout, "Internal Error: %s:%d: %s\n", file, line, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return pointer;
}
