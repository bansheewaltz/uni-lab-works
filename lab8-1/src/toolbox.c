#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_INTERNAL

#include "main.h"

void print_std_errorcode_info(char *file, int line, char *error_msg)
{
  char *executable_name = getenv("EXE_NAME");
  if (executable_name != NULL) {
    fprintf(stderr, "%s: ", executable_name);
  }
  char *msg = "\033[0;31merror:\033[0m";
  fprintf(stderr, "%s:%d: %s %s\n", file, line, msg, error_msg);
}

void *alarming_malloc(size_t size, char *file, int line)
{
  void *pointer = malloc(size);
  if (pointer == NULL) {
    print_std_errorcode_info(file, line, strerror(errno));
  }
  return pointer;
}

void *alarming_calloc(size_t count, size_t size, char *file, int line)
{
  void *pointer = calloc(count, size);
  if (pointer == NULL) {
    print_std_errorcode_info(file, line, strerror(errno));
  }
  return pointer;
}

bool is_any_null(int pointers_count, ...)
{
  bool result = false;
  va_list list;  // NOLINT
  va_start(list, pointers_count);

  for (int i = 0; i < pointers_count; ++i) {
    if (va_arg(list, void *) == NULL) { // NOLINT
      result = true;
    }
  }

  va_end(list);
  return result;
}
