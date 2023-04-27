// Dijkstra algorithm for undirected weighted graph
// goal: find the shortest path's length between two vertices

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dijkstra_algorithm.h"
#include "graph.h"
#include "input.h"
#include "output.h"
#include "utils.h"

int main(void) {
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  int V;
  int E;
  int src;
  int dst;

  if (scan_validate_parameters(&V, &src, &dst, &E) == SUCCESS) {
    Graph *graph = create_graph(V, E, UNDIRECTED);
#ifdef DEBUG
    print_graph(graph);
#endif

    PathInfo pathInfo = dijkstra_naive(graph, src, dst);
    print_path_info(&pathInfo, V, stdout);

    destroy_graph(graph);
    deallocate_path_info(&pathInfo);
  }

  return EXIT_SUCCESS;
}
