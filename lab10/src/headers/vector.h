#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

typedef struct TVector {
  int Count;
  int Max;
  int Size;
  void* Array;
} TVector;

TVector create_vector(int max, int size);
void assign_vector(int size, const void* src, void* dest);
void* get_nth_vector(TVector vector, int n);
TVector input_vector(int count, int size, int (*Scan)(void*));
void destroy_vector(TVector* vector);

#endif
