#include "main.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "utils.h"

#define malloc(n) safe_malloc(n, __FILE__, __LINE__)
#define calloc(n, size) safe_calloc(n, size, __FILE__, __LINE__)

enum { NOT_VISITED, TEMPORARY_MARK, PERMANENT_MARK };

bool topological_sort_recursive(bool *graph_array, int *vertex_state, int v,
                                int vertices_count, int *stack_array,
                                int *stack_top) {
  if (vertex_state[v] == PERMANENT_MARK) {
    return true;
  }
  if (vertex_state[v] == TEMPORARY_MARK) {
    return false;
  }
  vertex_state[v] = TEMPORARY_MARK;

  bool result = true;
  for (int i = vertices_count - 1; i >= 0; --i) {
    if (graph_array[vertices_count * v + i]) {
      result =
          topological_sort_recursive(graph_array, vertex_state, i,
                                     vertices_count, stack_array, stack_top);
      if (result == false) {
        break;
      }
    }
  }

  if (result == true) {
    vertex_state[v] = PERMANENT_MARK;
    stack_array[(*stack_top)++] = v + 1;
  }
  return result;
}

int *topological_sort(bool *graph_array, int vertices_count) {
  int *vertex_state = calloc((size_t)vertices_count, sizeof(int));
  int *stack_array = calloc((size_t)vertices_count, sizeof(int));
  int stack_top = 0;

  bool result = false;
  for (int i = 0; i < vertices_count; ++i) {
    if (vertex_state[i] == NOT_VISITED) {
      result =
          topological_sort_recursive(graph_array, vertex_state, i,
                                     vertices_count, stack_array, &stack_top);
    }
    if (result == false) {
      free(stack_array);
      stack_array = NULL;
      break;
    }
  }

  free(vertex_state);
  return stack_array;
}

int main(void) {
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif

  int vertices_count = 0;
  int edges_count = 0;
  bool *graph_array = NULL;

  if (!scan_validate_parameters(&vertices_count, &edges_count)) {
    goto cleanup_and_exit;
  }

  size_t graph_matrix_size = (size_t)vertices_count * (size_t)vertices_count;
  graph_array = calloc(graph_matrix_size, sizeof(bool));
  if (graph_array == NULL) {
    goto cleanup_and_exit;
  }

  if (!scan_edges(graph_array, vertices_count, edges_count)) {
    goto cleanup_and_exit;
  }

  int *sorted_nodes = NULL;
  if ((sorted_nodes = topological_sort(graph_array, vertices_count)) != NULL) {
    stack_array_print(sorted_nodes, vertices_count);
    free(sorted_nodes);
  } else {
    puts("impossible to sort");
  }

cleanup_and_exit:
  if (graph_array != NULL) {
    free(graph_array);
  }
  return EXIT_SUCCESS;
}
