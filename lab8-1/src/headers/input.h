#ifndef INPUT_H_
#define INPUT_H_

#include <stdbool.h>

#include "error.h"
#include "main.h"
typedef struct {
  int src;
  int dst;
  int weight;
} Edge;

ReturnCode scan_validate_vertices_count(int *vertices_count);
ReturnCode scan_validate_edges_count(int *edges_count, int vertices_count);
ReturnCode scan_validate_parameters(int *vertices_count, int *edges_count);
ReturnCode validate_edge(int src, int dst, int vertices_count);
ReturnCode scan_validate_edges(Graph *graph);

#endif  // INPUT_H_
