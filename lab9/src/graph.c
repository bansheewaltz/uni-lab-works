#include "graph.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "adjacency_list.h"
#include "adjacency_matrix.h"
#include "graph.h"
#include "utils.h"

void reverse_edge(Edge *edge) {
  assert(edge != NULL);

  int temp_dst = edge->dst;
  edge->dst = edge->src;
  edge->src = temp_dst;
}

bool is_graph_dense(int V, int E, bool directivity) {
  size_t adj_matrix_size;

  size_t adj_list_size;
  size_t list_nodes_size = E * sizeof(AdjListNode);
  size_t list_pointers_size = (V + 1) * sizeof(AdjListNode *);
  adj_list_size = list_nodes_size + list_pointers_size;

  if (directivity == DIRECTED) {
    adj_matrix_size = (V + 1) * (V + 1) * sizeof(int);
  } else {
    adj_matrix_size = E * sizeof(int);
  }

  size_t max_edges = V * (V - 1) / 2;
  float graph_density = (float)E / max_edges;

  return adj_matrix_size < adj_list_size && graph_density > DENSITY_CONSTANT;
}

Graph *create_graph(int V, int E, bool directivity) {
  Graph *graph = (Graph *)calloc(1, sizeof(Graph));
  if_fail(graph == NULL, __FILE__, __LINE__ - 1);

  graph->n_vertices = V;
  graph->n_edges = E;
  graph->directivity = directivity;

  if (!is_graph_dense(V, E, directivity)) {
    graph->representation = ADJACENCY_LIST;
    graph->adj_lists = create_graph_adj_lists(V, E, directivity);
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
