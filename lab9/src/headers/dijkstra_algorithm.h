#ifndef DIJKSTRA_ALGORITHM_H
#define DIJKSTRA_ALGORITHM_H

#include <inttypes.h>
#include <stdbool.h>

#include "graph.h"

#define INFINITY_LENGTH UINT64_MAX
#define UNDEFINED_NEIGHBOUR -1

typedef struct {
  int src;
  int dst;
  uint64_t *distances;
  int *previous_arr;
  int n_vertices;
} PathInfo;

PathInfo find_shortest_path_dijkstra_naive(Graph *graph, int src, int dst);

#endif  // DIJKSTRA_ALGORITHM_H
