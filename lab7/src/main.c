#include "main.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "typehandlers.h"
#include "utils.h"

enum VertexState { NOT_VISITED, TEMPORARY_MARK, PERMANENT_MARK };

bool topsort_recursive(Graph *graph, Stack *stack, int v, int *vertex_state) {
  int vertices_count = graph->vertices_count;
  bool *graph_array = graph->graph_array;

  if (vertex_state[v] == PERMANENT_MARK) {
    return true;
  }
  if (vertex_state[v] == TEMPORARY_MARK) {
    return false;
  }
  vertex_state[v] = TEMPORARY_MARK;

  bool result = true;
  for (int i = vertices_count - 1; i >= 0; --i) {
    if (!graph_array[vertices_count * v + i]) {
      continue;
    }
    result = topsort_recursive(graph, stack, i, vertex_state);
    if (result == false) {
      break;
    }
  }

  if (result == true) {
    vertex_state[v] = PERMANENT_MARK;
    result = stack_push(stack, v + 1);
  }
  return result;
}

Stack *graph_topological_sort(Graph *graph) {
  int vertices_count = graph->vertices_count;

  Stack *stack = stack_init(vertices_count);
  if (stack == NULL) {
    goto out;
  }

  int *vertex_state = calloc((size_t)vertices_count, sizeof(int));
  if (vertex_state == NULL) {
    goto out;
  }

  bool result = false;
  for (int i = 0; i < vertices_count; ++i) {
    if (vertex_state[i] == NOT_VISITED) {
      result = topsort_recursive(graph, stack, i, vertex_state);
    }
    if (result == false) {
      stack_free(stack);
      stack = NULL;
      break;
    }
  }

  free(vertex_state);
out:
  return stack;
}

int main(void) {
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  int vertices_count = 0;
  int edges_count = 0;
  Graph *graph = NULL;

  if (!graph_scan_validate_parameters(&vertices_count, &edges_count)) {
    goto cleanup_and_exit;
  }

  graph = graph_init(vertices_count, edges_count);
  if (graph == NULL || graph_scan_validate_edges(graph) == FAILURE) {
    goto cleanup_and_exit;
  }

  Stack *ordered_vertices = graph_topological_sort(graph);
  if (ordered_vertices != NULL) {
    stack_print(ordered_vertices);
    stack_free(ordered_vertices);
  } else {
    puts("impossible to sort");
  }

cleanup_and_exit:
  graph_free(graph);
  return EXIT_SUCCESS;
}
