#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#include "main.h"

#define SUCCESS true
#define FAILURE false

#define is_in_range(n, l, r) ((l) <= (n) && (n) <= (r))

#ifndef F_MEMORY_INTERNAL
#define malloc(n) safe_malloc(n, __FILE__, __LINE__)
#define calloc(n, size) safe_calloc(n, size, __FILE__, __LINE__)
#endif

void print_error(char *file, int line, char *error_msg);
void *safe_malloc(size_t size, char *file, int line);
void *safe_calloc(size_t n, size_t size, char *file, int line);

#endif  // UTILS_H
