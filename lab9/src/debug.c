#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"

void print_adj_matrix_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  int n_vertices = graph->n_vertices;
  printf("x| ");
  for (int i = 1; i <= n_vertices; ++i) {
    printf("%d ", i);
  }
  printf("\n");

  for (int r = 1; r <= n_vertices; ++r) {
    printf("%d| ", r);

    for (int c = 1; c <= n_vertices; ++c) {
      printf("%d ", graph->adjacency_matrix[r * (n_vertices + 1) + c]);
    }
    printf("\n");
  }
}

void print_adj_list_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  for (int i = 1; i <= graph->n_vertices; ++i) {
    AdjListNode *ptr = graph->adjacency_lists[i];

    while (ptr != NULL) {
      printf("%d —> %d (%d)\t", i, ptr->dst, ptr->length);
      ptr = ptr->next;
    }

    printf("\n");
  }
}

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
