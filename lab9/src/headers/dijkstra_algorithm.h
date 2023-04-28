#ifndef DIJKSTRA_ALGORITHM_H
#define DIJKSTRA_ALGORITHM_H

#include <inttypes.h>
#include <stdbool.h>

#include "graph.h"

typedef struct PathInfo PathInfo;
struct PathInfo {
  int src;
  int dst;
  uint64_t *distances;
  int *previous_arr;
};

int get_min_dist_v(bool const visited[], uint64_t const dist[], int n_vertices);
void relaxate_neighbours(Graph *graph, uint64_t dist[], bool const visited[],
                         int previous_v[], int min_dist_v);
PathInfo dijkstra_naive(Graph *graph, int src, int dst);

#endif  // DIJKSTRA_ALGORITHM_H
