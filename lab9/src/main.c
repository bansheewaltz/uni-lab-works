// Dijkstra algorithm for undirected weighted graph
// goal: find the shortest path's length between two vertices

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* changable variables */
#define MAX_VERTICES_NUM 5000
#define MAX_EDGES_NUM (N * (N - 1) / 2)
#define MAX_EDGE_LEN INT_MAX
/* utility stuff */
#define SUCCESS 1
#define FAILURE 0
#define INFINITY INT64_MAX
#define UNDEFINED -1
#define IS_IN_BOUNDS(l, value, r) (l <= value && value <= r)

typedef struct Graph Graph;
typedef struct AdjListNode AdjListNode;
typedef struct PathInfo PathInfo;

struct Graph {
  int n_vertices;
  int n_edges;
  AdjListNode **adjacency_lists;
};

struct AdjListNode {
  int dst;
  int64_t length;
  AdjListNode *next;
};

struct PathInfo {
  int src;
  int dst;
  int64_t *distances;
  int *previous_arr;
};

void print_error_terminate(char message[]) {
  puts(message);
  exit(EXIT_SUCCESS);  // but actually FAILURE
}

void add_node(Graph *graph, int src, int dst, int length) {
  if (graph == NULL) {
    return;
  }

  AdjListNode *new_node = (AdjListNode *)malloc(sizeof(AdjListNode));
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

void print_graph(Graph *graph, int M) {
  if (graph == NULL) {
    return;
  }

  for (int i = 1; i < M + 1; i++) {
    AdjListNode *ptr = graph->adjacency_lists[i];

    while (ptr != NULL) {
      printf("%d —> %d (%" PRId64 ")\t", i, ptr->dst, ptr->length);
      ptr = ptr->next;
    }

    printf("\n");
  }
}

bool scan_validate_n_vertices(int *N) {
  if (scanf("%i", N) && IS_IN_BOUNDS(0, *N, MAX_VERTICES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of vertices");
  return false;
}

bool scan_validate_path(int *S, int *F, int N) {
  if ((scanf("%d", S) && IS_IN_BOUNDS(1, *S, N)) &&
      (scanf("%d", F) && IS_IN_BOUNDS(1, *F, N))) {
    return true;
  }

  print_error_terminate("bad vertex");
  return false;
}

bool scan_validate_n_edges(int *M, int N) {
  if (scanf("%d", M) && IS_IN_BOUNDS(0, *M, MAX_EDGES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of edges");
  return false;
}

bool scan_validate_edge_len(int64_t *length) {
  if (scanf("%" SCNd64, length) && IS_IN_BOUNDS(0, *length, MAX_EDGE_LEN)) {
    return true;
  }

  print_error_terminate("bad length");
  return false;
}

int scan_validate_edge(int *src, int *dst, int N, int64_t *length) {
  if (scan_validate_path(src, dst, N) && scan_validate_edge_len(length)) {
    return SUCCESS;
  }

  return FAILURE;
}

bool scan_validate_parameters(int *N, int *S, int *F, int *M) {
  return scan_validate_n_vertices(N) &&   //
         scan_validate_path(S, F, *N) &&  //
         scan_validate_n_edges(M, *N);    //
}

Graph *create_graph(int N, int M) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));

  graph->n_vertices = N;
  graph->n_edges = M;
  graph->adjacency_lists =  // N + 1 because idx 0 node will be ingored
      (AdjListNode **)malloc((N + 1) * sizeof(AdjListNode *));

  initialise_lists(graph->adjacency_lists, N + 1);

  for (int i = 0; i < M; ++i) {
    int src;
    int dst;
    int64_t length;

    bool ret = scan_validate_edge(&src, &dst, N, &length);
    if (ret != SUCCESS) {
      print_error_terminate("bad number of lines");
    }

    add_node(graph, src, dst, length);
    add_node(graph, dst, src, length);
  }

  return graph;
}

void destroy_graph(Graph *graph) {
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
  free(graph);
}

int get_min_dist_v(bool const visited[], int64_t const dist[], int n_vertices) {
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

void relaxate_set_previous(AdjListNode *neighbour, int64_t dist[],
                           int previous_v[], int min_dist_v) {
  int neighbour_v = neighbour->dst;
  int rel_dist_to_neighbour = neighbour->length;
  int64_t calculated_distance = dist[min_dist_v] + rel_dist_to_neighbour;

  if (calculated_distance < dist[neighbour_v]) {
    dist[neighbour_v] = calculated_distance;
    previous_v[neighbour_v] = min_dist_v;
  }
}

PathInfo dijkstra_naive_adj_list(Graph *graph, int S, int F) {
  int n_vertices = graph->n_vertices;
  int arr_size = n_vertices + 1;
  bool *visited = (bool *)calloc(arr_size, sizeof(bool));

  int64_t *dist = (int64_t *)malloc(sizeof(int64_t) * arr_size);
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
    AdjListNode *neighbour = graph->adjacency_lists[min_dist_v];
    while (neighbour != NULL) {
      if (!visited[neighbour->dst]) {
        relaxate_set_previous(neighbour, dist, previous_v, min_dist_v);
      }
      neighbour = neighbour->next;
    }
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
    int64_t distance_from_source = pathInfo->distances[i];
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

  int64_t src_to_dst_len = pathInfo->distances[pathInfo->dst];
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
    Graph *graph = create_graph(N, M);
#ifdef DEBUG
    print_graph(graph, N);
#endif

    PathInfo pathInfo = dijkstra_naive_adj_list(graph, S, F);
    print_path_info(&pathInfo, N, stdout);

    destroy_graph(graph);
    deallocate_path_info(&pathInfo);
  }

  return EXIT_SUCCESS;
}
