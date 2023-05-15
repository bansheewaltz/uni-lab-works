#include "main.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define VERTICES_MAX_COUNT   2000
#define is_in_range(n, l, r) ((l) <= (n) && (n) <= (r))
#define malloc(n)            safe_malloc(n, __FILE__, __LINE__)
#define calloc(n, size)      safe_calloc(n, size, __FILE__, __LINE__)

enum { NOT_VISITED, TEMPORARY_MARK, PERMANENT_MARK };

int scan_validate_vertices_count(void) {
  int temp = 0;
  if (!scanf("%d", &temp) || !is_in_range(temp, 0, VERTICES_MAX_COUNT)) {
    puts("bad number of vertices");
    exit(EXIT_SUCCESS);
  }
  return temp;
}

int scan_validate_edges_count(int vertices_count) {
  int temp = 0;
  const int EDGES_MAX_COUNT = vertices_count * (vertices_count - 1) / 2;

  if (!scanf("%d", &temp) || !is_in_range(temp, 0, EDGES_MAX_COUNT)) {
    puts("bad number of edges");
    exit(EXIT_SUCCESS);
  }

  return temp;
}

bool validate_edge(int src, int dst, int vertices_count) {
  if (is_in_range(src, 1, vertices_count) &&
      is_in_range(dst, 1, vertices_count)) {
    return true;
  }
  return false;
}

void scan_edges(bool *graph_array, int vertices_count, int edges_count) {
  int edge_src = 0;
  int edge_dst = 0;
  int scanned_edges_count = 0;

  while (scanned_edges_count < edges_count) {
    int scanned = scanf("%d %d", &edge_src, &edge_dst);

    if (scanned == 2 && validate_edge(edge_src, edge_dst, vertices_count)) {
      graph_array[--edge_src * vertices_count + --edge_dst] = 1;
    } else {
      puts("bad vertex");
      exit(EXIT_SUCCESS);
    }

    ++scanned_edges_count;
  }

  if (scanned_edges_count < edges_count) {
    puts("bad number of lines");
    exit(EXIT_SUCCESS);
  }
}

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
  int *stack_array = malloc(sizeof(uint) * (size_t)vertices_count);
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

void stack_array_print(int *stack_array, int length) {
  for (int i = length; i >= 0; --i) {
    if (stack_array[i] != 0) {
      printf("%d%s", stack_array[i], i == 0 ? "" : " ");
    }
  }
}

// bool scan_validate_parameters(int &vertices_count, int &edges_count) {
// }

int main(void) {
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  int vertices_count = scan_validate_vertices_count();
  int edges_count = scan_validate_edges_count(vertices_count);
  size_t graph_matrix_size = (size_t)vertices_count * (size_t)vertices_count;
  bool *graph_array = calloc(graph_matrix_size, sizeof(bool));
  scan_edges(graph_array, vertices_count, edges_count);

  int *sorted_nodes = NULL;
  if ((sorted_nodes = topological_sort(graph_array, vertices_count))) {
    stack_array_print(sorted_nodes, vertices_count);
    free(sorted_nodes);
  } else {
    puts("impossible to sort");
  }

  free(graph_array);
  return EXIT_SUCCESS;
}
