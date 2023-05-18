#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_INTERNAL

#include "main.h"

void print_error(char *file, int line, char *error_msg)
{
  char *executable_name = getenv("EXE_NAME");
  if (executable_name != NULL) {
    fprintf(stderr, "%s: ", executable_name);
  }
  char *msg = "\033[0;31merror:\033[0m";
  fprintf(stderr, "%s:%d: %s %s\n", file, line, msg, error_msg);
}

void *safe_malloc(size_t size, char *file, int line)
{
  void *pointer = malloc(size);
  if (pointer == NULL) {
    print_error(file, line, strerror(errno));
  }
  return pointer;
}

void *safe_calloc(size_t n, size_t size, char *file, int line)
{
  void *pointer = calloc(n, size);
  if (pointer == NULL) {
    print_error(file, line, strerror(errno));
  }
  return pointer;
}
