// Dijkstra algorithm for undirected weighted graph
// goal: find the shortest path's length between two vertices

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dijkstra_algorithm.h"
#include "input.h"
#include "matrix.h"
#include "output.h"
#include "typedefs.h"
#include "utils.h"

void add_adj_list_node(AdjListNode **adj_lists, Edge *edge) {
  if (adj_lists == NULL) {
    return;
  }

  AdjListNode *new_node = (AdjListNode *)malloc(sizeof(AdjListNode));
  if (new_node == NULL) {
    print_error_terminate("heap buffer overflow");
    // print_error_terminate(__FILE__, __LINE__, "allocation failed");
  }

  new_node->dst = edge->dst;
  new_node->length = edge->weight;
  new_node->next = adj_lists[edge->src];

  adj_lists[edge->src] = new_node;
}

void initialise_lists(AdjListNode *adj_lists[], int N) {
  if (adj_lists == NULL) {
    return;
  }

  for (int i = 0; i < N; i++) {
    adj_lists[i] = NULL;
  }
}

void reverse_edge(Edge *edge) {
  int temp_dst = edge->dst;
  edge->dst = edge->src;
  edge->src = temp_dst;
}

AdjListNode **create_graph_adj_lists(int n_vertices, int n_edges,
                                     bool directivity) {
  AdjListNode **adj_lists;  // N + 1 b/c idx 0 node will be ingored
  adj_lists = (AdjListNode **)malloc((n_vertices + 1) * sizeof(AdjListNode *));
  if (adj_lists == NULL) {
    print_error_terminate("heap buffer overflow");
  }

  initialise_lists(adj_lists, n_vertices + 1);

  for (int i = 0; i < n_edges; ++i) {
    Edge edge;

    bool ret = scan_validate_edge(&edge, n_vertices);
    if (ret != SUCCESS) {
      print_error_terminate("bad number of lines");
    }

    add_adj_list_node(adj_lists, &edge);
    if (directivity == UNDIRECTED) {
      reverse_edge(&edge);
      add_adj_list_node(adj_lists, &edge);
    }
  }

  return adj_lists;
}

bool is_graph_dense(int n_vertices, int n_edges) {
  int max_edges = n_vertices * (n_vertices - 1) / 2;
  return (float)n_edges / max_edges > DENSE_COEFFICIENT;
}

Graph *create_graph(int N, int M, bool directivity) {
  Graph *graph = (Graph *)calloc(1, sizeof(Graph));
  if (graph == NULL) {
    print_error_terminate("heap buffer overflow");
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

void deallocate_adj_list(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  for (int i = 0; i < graph->n_vertices + 1; ++i) {
    AdjListNode *list_head = graph->adj_lists[i];

    if (list_head != NULL) {
      AdjListNode *tmp_next = list_head->next;

      while (tmp_next != NULL) {
        free(list_head);
        list_head = tmp_next;
        tmp_next = list_head->next;
      }
      free(list_head);
    }
  }

  free(graph->adj_lists);
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

void deallocate_path_info(PathInfo *pathInfo) {
  free(pathInfo->distances);
  free(pathInfo->previous_arr);
}

int main(void) {
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  int N;  // number of vertices in the graph
  int S;  // path source
  int F;  // path destination
  int M;  // number of edges in the graph

  if (scan_validate_parameters(&N, &S, &F, &M) == SUCCESS) {
    Graph *graph = create_graph(N, M, UNDIRECTED);
#ifdef DEBUG
    print_graph(graph);
#endif

    PathInfo pathInfo = dijkstra_naive(graph, S, F);
    print_path_info(&pathInfo, N, stdout);

    destroy_graph(graph);
    deallocate_path_info(&pathInfo);
  }

  return EXIT_SUCCESS;
}
