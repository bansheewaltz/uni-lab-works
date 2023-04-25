// Dijkstra algorithm for undirected weighted graph
// goal: find the shortest path's length between two vertices

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "typedefs.h"
#include "utils.h"

#ifdef DEBUG
#include "debug.h"
#endif

void add_adj_list_node(Graph *graph, int src, int dst, int length) {
  if (graph == NULL) {
    return;
  }

  AdjListNode *new_node = NULL;
  new_node = (AdjListNode *)malloc(sizeof(AdjListNode));
  if (new_node == NULL) {
    print_error_terminate("heap buffer overflow");
  }

  new_node->dst = dst;
  new_node->length = length;
  new_node->next = graph->adjacency_lists[src];

  graph->adjacency_lists[src] = new_node;
}

void initialise_lists(AdjListNode *adjacency_lists[], int N) {
  if (adjacency_lists == NULL) {
    return;
  }

  for (int i = 0; i < N; i++) {
    adjacency_lists[i] = NULL;
  }
}

Graph *create_graph_adj_list(int N, int M, bool directivity) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));

  graph->representation = ADJACENCY_LIST;
  graph->n_vertices = N;
  graph->n_edges = M;
  graph->adjacency_lists =  // N + 1 because idx 0 node will be ingored
      (AdjListNode **)malloc((N + 1) * sizeof(AdjListNode *));

  initialise_lists(graph->adjacency_lists, N + 1);

  for (int i = 0; i < M; ++i) {
    int src;
    int dst;
    int length;

    bool ret = scan_validate_edge(&src, &dst, N, &length);
    if (ret != SUCCESS) {
      print_error_terminate("bad number of lines");
    }

    add_adj_list_node(graph, src, dst, length);
    if (directivity == UNDIRECTED) {
      add_adj_list_node(graph, dst, src, length);
    }
  }

  return graph;
}

void add_adj_matrix_entry(Graph *graph, int src, int dst, int length) {
  graph->adjacency_matrix[(graph->n_vertices + 1) * src + dst] = length;
}

Graph *create_graph_adj_matrix(int N, int M, bool directivity) {
  Graph *graph = (Graph *)calloc(1, sizeof(Graph));

  graph->representation = ADJACENCY_MATRIX;
  graph->n_vertices = N;
  graph->n_edges = M;
  graph->adjacency_matrix =  // N + 1 because idx 0 node will be ingored
      (int *)calloc(((N + 1) * (N + 1)), sizeof(int));

  for (int i = 0; i < M; ++i) {
    int src;
    int dst;
    int length;

    bool ret = scan_validate_edge(&src, &dst, N, &length);
    if (ret != SUCCESS) {
      print_error_terminate("bad number of lines");
    }

    add_adj_matrix_entry(graph, src, dst, length);
    if (directivity == UNDIRECTED) {
      add_adj_matrix_entry(graph, dst, src, length);
    }
  }

  return graph;
}

Graph *create_graph(int N, int M, bool directivity) {
  Graph *graph;

  if (N * (N - 1) * 0.25 > M) {
    graph = create_graph_adj_list(N, M, directivity);
  } else {
    graph = create_graph_adj_matrix(N, M, directivity);
  }

  return graph;
}

void deallocate_adj_list(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  for (int i = 0; i < graph->n_vertices + 1; ++i) {
    AdjListNode *list_head = graph->adjacency_lists[i];

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

  free(graph->adjacency_lists);
}

void destroy_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  if (graph->representation == ADJACENCY_LIST) {
    deallocate_adj_list(graph);
  }
  if (graph->representation == ADJACENCY_MATRIX) {
    free(graph->adjacency_matrix);
  }
  free(graph);
}

int get_min_dist_v(bool const visited[], uint64_t const dist[],
                   int n_vertices) {
  int min_dist_v = 1;

  for (int i = 1; i <= n_vertices; ++i) {
    if (!visited[i]) {
      min_dist_v = i;
      break;
    }
  }

  for (int i = min_dist_v; i <= n_vertices; ++i) {
    if (!visited[i] && dist[i] < dist[min_dist_v]) {
      min_dist_v = i;
    }
  }

  return min_dist_v;
}

int get_matrix_entry_idx(Graph *graph, int row, int column) {
  return row * (graph->n_vertices + 1) + column;
}

int get_matrix_entry(Graph *graph, int row, int column) {
  return graph->adjacency_matrix[get_matrix_entry_idx(graph, row, column)];
}

bool is_matrix_neighbour(Graph *graph, int row, int column) {
  return get_matrix_entry(graph, row, column) != 0;
}

void relaxate_neighbours(Graph *graph, uint64_t dist[], bool const visited[],
                         int previous_v[], int min_dist_v) {
  if (graph->representation == ADJACENCY_LIST) {
    AdjListNode *neighbour = graph->adjacency_lists[min_dist_v];

    while (neighbour != NULL) {
      if (!visited[neighbour->dst]) {
        int neighbour_v = neighbour->dst;
        int rel_dist_to_neighbour = neighbour->length;
        uint64_t calculated_distance = dist[min_dist_v] + rel_dist_to_neighbour;

        if (calculated_distance < dist[neighbour_v]) {
          dist[neighbour_v] = calculated_distance;
          previous_v[neighbour_v] = min_dist_v;
        }
      }
      neighbour = neighbour->next;
    }
  }

  if (graph->representation == ADJACENCY_MATRIX) {
    for (int c = 1; c <= graph->n_vertices; ++c) {
      if (!visited[c] && is_matrix_neighbour(graph, min_dist_v, c)) {
        int rel_dist_to_neighbour = get_matrix_entry(graph, min_dist_v, c);
        uint64_t calculated_distance = dist[min_dist_v] + rel_dist_to_neighbour;

        if (calculated_distance < dist[c]) {
          dist[c] = calculated_distance;
          previous_v[c] = min_dist_v;
        }
      }
    }
  }
}

PathInfo dijkstra_naive(Graph *graph, int S, int F) {
  int n_vertices = graph->n_vertices;
  int arr_size = n_vertices + 1;
  bool *visited = (bool *)calloc(arr_size, sizeof(bool));

  uint64_t *dist = (uint64_t *)malloc(sizeof(uint64_t) * arr_size);
  int *previous_v = (int *)malloc(sizeof(int) * arr_size);
  for (int i = 0; i <= n_vertices; ++i) {
    dist[i] = INFINITY;
    previous_v[i] = UNDEFINED;
  }
  dist[S] = 0;

  for (int n_visited = 0; n_visited < n_vertices; ++n_visited) {
    int min_dist_v = get_min_dist_v(visited, dist, n_vertices);
    if (dist[min_dist_v] == INFINITY) {
      break;
    }

    visited[min_dist_v] = true;
    relaxate_neighbours(graph, dist, visited, previous_v, min_dist_v);
  }

  free(visited);
  return (PathInfo){S, F, dist, previous_v};
}

void deallocate_path_info(PathInfo *pathInfo) {
  free(pathInfo->distances);
  free(pathInfo->previous_arr);
}

void print_path_info(PathInfo *pathInfo, int n_vertices, FILE *output) {
  int overflow_counter = 0;

  for (int i = 1; i <= n_vertices; ++i) {
    uint64_t distance_from_source = pathInfo->distances[i];
    if (distance_from_source == INFINITY) {
      fprintf(output, "oo ");
      continue;
    }

    if (distance_from_source >= INT_MAX) {
      ++overflow_counter;
    }
    if (distance_from_source > INT_MAX) {
      fprintf(output, "INT_MAX+ ");
    } else {
      fprintf(output, "%" PRId64 " ", distance_from_source);
    }
  }
  fprintf(output, "\n");

  uint64_t src_to_dst_len = pathInfo->distances[pathInfo->dst];
  if (src_to_dst_len == INFINITY) {
    fprintf(output, "no path");
  } else if (src_to_dst_len > INT_MAX && overflow_counter > 2) {
    fprintf(output, "overflow");
  } else {
    int temp = pathInfo->dst;
    while (temp != -1) {
      printf("%d ", temp);
      temp = pathInfo->previous_arr[temp];
    }
  }
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
