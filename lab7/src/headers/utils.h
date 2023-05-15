#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#include "main.h"

void *safe_malloc(size_t size, char *file, uint line);
void *safe_calloc(size_t n, size_t size, char *file, uint line);

#endif  // UTILS_H
