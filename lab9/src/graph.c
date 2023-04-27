#include "graph.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "adjacency_list.h"
#include "adjacency_matrix.h"
#include "typedefs.h"
#include "utils.h"

void reverse_edge(Edge *edge) {
  assert(edge != NULL);

  int temp_dst = edge->dst;
  edge->dst = edge->src;
  edge->src = temp_dst;
}

bool is_graph_dense(int n_vertices, int n_edges) {
  int max_edges = n_vertices * (n_vertices - 1) / 2;
  return (float)n_edges / max_edges > DENSE_COEFFICIENT;
}

Graph *create_graph(int N, int M, bool directivity) {
  Graph *graph = (Graph *)calloc(1, sizeof(Graph));
  if (graph == NULL) {
    print_allocation_failed_terminate(__FILE__, __LINE__ - 2);
  }

  graph->n_vertices = N;
  graph->n_edges = M;
  graph->directivity = directivity;

  if (!is_graph_dense(N, M)) {
    graph->representation = ADJACENCY_LIST;
    graph->adj_lists = create_graph_adj_lists(N, M, directivity);
  } else {
    graph->representation = ADJACENCY_MATRIX;
    graph->adj_matrix = create_graph_adj_matrix(graph);
  }

  return graph;
}

void destroy_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  if (graph->representation == ADJACENCY_LIST) {
    deallocate_adj_list(graph);
  }
  if (graph->representation == ADJACENCY_MATRIX) {
    free(graph->adj_matrix);
  }
  free(graph);
}

// cppcheck-suppress unusedFunction
void print_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  if (graph->representation == ADJACENCY_LIST) {
    print_adj_list_graph(graph);
  }
  if (graph->representation == ADJACENCY_MATRIX) {
    print_adj_matrix_graph(graph);
  }
}
