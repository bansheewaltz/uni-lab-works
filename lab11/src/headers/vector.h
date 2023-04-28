#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
  int *array;
  size_t size;
  size_t capacity;
} Vector;

void vector_init(Vector *vector, size_t initial_size);
void vector_insert(Vector *vector, int element);
void vector_free(Vector *vector);

#endif  // VECTOR_H
