#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "input.h"
#include "typedefs.h"
#include "utils.h"

int get_nth_triangular_number(int number) {
  return (1 + number) * number / 2;
}

int get_adj_matrix_entry_idx(Graph *graph, int row, int column) {
  assert(graph != NULL);

  if (graph->directivity == DIRECTED) {
    return (graph->n_vertices + 1) * row + column;
  } else {
    int greater = row > column ? row : column;
    int smaller = row < column ? row : column;
    return get_nth_triangular_number(greater - 2) + smaller - 1;
  }
}

int get_adj_matrix_entry(Graph *graph, int row, int column) {
  assert(graph != NULL);
  assert(graph->adj_matrix != NULL);

  if (row == column) {
    return 0;
  }

  return graph->adj_matrix[get_adj_matrix_entry_idx(graph, row, column)];
}

void add_adj_matrix_entry(Graph *graph, Edge *edge) {
  assert(graph != NULL);
  assert(edge != NULL);
  assert(graph->adj_matrix != NULL);

  if (edge->dst == edge->src) {
    return;
  }

  int entry_id = get_adj_matrix_entry_idx(graph, edge->src, edge->dst);

  graph->adj_matrix[entry_id] = edge->weight;
}

bool is_matrix_neighbour(Graph *graph, int row, int column) {
  assert(graph != NULL);

  return get_adj_matrix_entry(graph, row, column) != 0;
}

int *create_graph_adj_matrix(Graph *graph) {
  assert(graph != NULL);

  int n_vertices = graph->n_vertices;
  int *adj_matrix;
  int array_size;

  if (graph->directivity == DIRECTED) {
    array_size = n_vertices * n_vertices;
  } else {
    array_size = get_nth_triangular_number(n_vertices - 1);
  }
  adj_matrix = (int *)calloc(array_size, sizeof(int));
  if (adj_matrix == NULL) {
    print_error_terminate("heap buffer overflow");
  }
  graph->adj_matrix = adj_matrix;

  for (int i = 0; i < graph->n_edges; ++i) {
    Edge edge;

    bool ret = scan_validate_edge(&edge, n_vertices);
    if (ret != SUCCESS) {
      print_error_terminate("bad number of lines");
    }

    add_adj_matrix_entry(graph, &edge);
  }

  return adj_matrix;
}

void print_adj_matrix_graph(Graph *graph) {
  assert(graph != NULL);
  assert(graph->adj_matrix != NULL);

  int n_vertices = graph->n_vertices;
  printf("x| ");
  for (int i = 1; i <= n_vertices; ++i) {
    printf("%d ", i);
  }
  printf("\n");

  for (int r = 1; r <= n_vertices; ++r) {
    printf("%d| ", r);

    for (int c = 1; c <= n_vertices; ++c) {
      if (graph->directivity == DIRECTED || r > c) {
        printf("%d ", get_adj_matrix_entry(graph, r, c));
      } else {
        printf("%d ", 0);
      }
    }
    printf("\n");
  }
}
