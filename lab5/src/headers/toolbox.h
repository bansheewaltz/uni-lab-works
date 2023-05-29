#ifndef UTILS_H_
#define UTILS_H_

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SUCCESS true
#define FAILURE false

#define is_in_range(n, l, r) ((l) <= (n) && (n) <= (r))

#ifndef MEMORY_INTERNAL
#define malloc(size) alarming_malloc(size, __FILE__, __LINE__)
#define calloc(count, size) alarming_calloc(count, size, __FILE__, __LINE__)
#endif

void print_std_errorcode_info(char *file, int line, char *error_msg);
void *alarming_malloc(size_t size, char *file, int line);
void *alarming_calloc(size_t count, size_t size, char *file, int line);
bool is_any_null(int pointers_count, ...);

#endif  // UTILS_H_
