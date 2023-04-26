#include "utils.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "typedefs.h"

void print_error_terminate(char message[]) {
  puts(message);
  exit(EXIT_SUCCESS);  // but actually FAILURE
}

void print_allocation_failed_terminate(char *file, int line) {
  fprintf(stdout, "Internal Error: %s:%d: %s\n", file, line, strerror(errno));
  exit(17);
}

int get_nth_triangular_number(int number) {
  return (1 + number) * number / 2;
}

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
      if (graph->directivity == DIRECTED || r > c) {
        printf("%d ", get_adj_matrix_entry(graph, r, c));
      } else {
        printf("%d ", 0);
      }
    }
    printf("\n");
  }
}

void print_adj_list_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  for (int i = 1; i <= graph->n_vertices; ++i) {
    AdjListNode *ptr = graph->adj_lists[i];

    while (ptr != NULL) {
      printf("%d —> %d (%d)\t", i, ptr->dst, ptr->length);
      ptr = ptr->next;
    }

    printf("\n");
  }
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
