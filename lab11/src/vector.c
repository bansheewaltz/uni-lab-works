#include "vector.h"

#include <stddef.h>
#include <stdlib.h>

void vector_init(Vector *vector, size_t initial_size) {
  vector->array = malloc(initial_size * sizeof(int));
  vector->size = 0;
  vector->capacity = initial_size;
}

void vector_insert(Vector *vector, int element) {
  if (vector->size == vector->capacity) {
    vector->capacity *= 2;
    vector->array = realloc(vector->array, vector->capacity * sizeof(int));
  }
  vector->array[vector->size++] = element;
}

void vector_free(Vector *vector) {
  free(vector->array);
  vector->array = NULL;
  vector->size = vector->capacity = 0;
}
