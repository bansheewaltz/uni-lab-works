#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

#define VERTICES_MAX_COUNT 2000
typedef struct {
  int vertices_count;
  int edges_count;
  bool *graph_array;
} Graph;

typedef struct {
  int *stack_array;
  int stack_top;
  int stack_capacity;
} Stack;

#endif  // MAIN_H
