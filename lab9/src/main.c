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
#define IS_IN_BOUNDS(l, value, r) (l <= value && value <= r)

typedef struct Graph Graph;
typedef struct AdjListNode AdjListNode;
typedef struct PathInfo PathInfo;

struct Graph {
  int num_of_vertices;
  int num_of_edges;
  AdjListNode **adjacency_lists;
};

struct AdjListNode {
  int dst;
  int64_t length;
  AdjListNode *next;
};

struct Edge {
  int src;
  int dst;
  int64_t length;
};

struct PathInfo {
  int num_of_vertices;
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

Graph *create_graph(struct Edge *edges[], int N, int M) {
  if (edges == NULL) {
    print_error_terminate("edges pointer is NULL");
  }

  Graph *graph = (Graph *)malloc(sizeof(Graph));

  graph->num_of_vertices = N;
  graph->num_of_edges = M;
  graph->adjacency_lists =  // N + 1 because idx 0 node will be ingored
      (AdjListNode **)malloc((N + 1) * sizeof(AdjListNode *));

  initialise_lists(graph->adjacency_lists, N + 1);

  for (int i = 0; i < M; ++i) {
    int src = edges[i]->src;
    int dst = edges[i]->dst;
    int length = edges[i]->length;

    add_node(graph, src, dst, length);
    add_node(graph, dst, src, length);
  }

  return graph;
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

bool scan_validate_num_of_vertices(int *N) {
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

bool scan_validate_num_of_edges(int *M, int N) {
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
  return scan_validate_num_of_vertices(N) &&  //
         scan_validate_path(S, F, *N) &&      //
         scan_validate_num_of_edges(M, *N);   //
}

bool scan_validate_input(int *N, int *S, int *F, int *M, struct Edge ***edges) {
  if (scan_validate_parameters(N, S, F, M) == FAILURE) {
    return FAILURE;
  }

  *edges = (struct Edge **)malloc(sizeof(struct Edge *) * (*M));
  if (*edges == NULL) {
    print_error_terminate("heap buffer overflow");
  }

  for (int i = 0; i < *M; ++i) {
    (*edges)[i] = (struct Edge *)malloc(sizeof(struct Edge));
    int *src = &(*edges)[i]->src;
    int *dst = &(*edges)[i]->dst;
    int64_t *len = &(*edges)[i]->length;

    bool ret = scan_validate_edge(src, dst, *N, len);
    if (ret != SUCCESS) {
      print_error_terminate("bad number of lines");
    }
  }

  return SUCCESS;
}

void destroy_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  for (int i = 0; i < graph->num_of_vertices + 1; ++i) {
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

void deallocate_temporary_storage(struct Edge **edges, int M) {
  if (edges == NULL) {
    return;
  }

  for (int i = 0; i < M; ++i) {
    free(edges[i]);
  }
  free(edges);
}

PathInfo find_shortest_path_by_dijkstra(Graph *graph, int S, int F) {
  int num_of_vertices = graph->num_of_vertices;

  // initialize distances and previous arrays
  int *previous_v = (int *)malloc(sizeof(int) * num_of_vertices);
  int64_t *known_dist = (int64_t *)malloc(sizeof(int64_t) * num_of_vertices);
  for (int i = 0; i < num_of_vertices; ++i) {
    known_dist[i] = INFINITY;
    previous_v[i] = -1;
  }
  known_dist[S] = 0;

  // initialize visited
  bool *visited = (bool *)calloc(num_of_vertices, sizeof(bool));

  // algorithm
  int visited_num = 0;
  int min_dist_v = S;
  while (visited_num < num_of_vertices) {
    // find vertex with smallest known_dist
    for (int i = 1; i < num_of_vertices; ++i) {
      if (!visited[i] && known_dist[i] < known_dist[min_dist_v]) {
        min_dist_v = i;
      }
    }
    // find known_dist to neighbour
    /// get adjacency_list for min_dist_v
    AdjListNode *neighbour_node = graph->adjacency_lists[min_dist_v];
    /// iterate over neighbour_node
    while (neighbour_node->next != NULL) {
      if (visited[neighbour_node->dst] == true) {
        continue;
      }

      int neighbour_v = neighbour_node->dst;
      int neighbour_dist = neighbour_node->length;
      int64_t calculated_distance = known_dist[min_dist_v] + neighbour_dist;

      if (calculated_distance < known_dist[neighbour_v]) {
        known_dist[neighbour_v] = calculated_distance;
        previous_v[neighbour_v] = min_dist_v;
      }
    }
    visited[min_dist_v] = true;
    ++visited_num;
  }

  free(visited);
  return (PathInfo){.num_of_vertices = num_of_vertices,
                    .src = S,
                    .dst = F,
                    .distances = known_dist,
                    .previous_arr = previous_v};
}

void deallocate_path_info(PathInfo *pathInfo) {
  free(pathInfo->distances);
  free(pathInfo->previous_arr);
}

void print_path_info(PathInfo *pathInfo, FILE *output) {
  for (int i = 1; i <= pathInfo->num_of_vertices; ++i) {
    int64_t distance_from_source = pathInfo->distances[i];
    if (distance_from_source == INFINITY) {
      fprintf(output, "oo ");
      continue;
    }
    if (distance_from_source > INT_MAX) {
      fprintf(output, "INT_MAX+ ");
      continue;
    }
    fprintf(output, "% " PRId64, distance_from_source);
  }
  fprintf(output, "\n");

  int64_t src_to_dst_len = pathInfo->distances[pathInfo->dst];
  if (src_to_dst_len > INT_MAX) {
    fprintf(output, "no path");
  } else {
    fprintf(output, "%" PRId64, src_to_dst_len);
  }
}

// the graph is not being built until all the input data is checked
int main(void) {
  int N;  // number of vertices in the graph
  int S;  // path source
  int F;  // path destination
  int M;  // number of edges in the graph
  struct Edge **edges_temp_storage = NULL;

  if (scan_validate_input(&N, &S, &F, &M, &edges_temp_storage) == SUCCESS) {
    Graph *graph = create_graph(edges_temp_storage, N, M);
    deallocate_temporary_storage(edges_temp_storage, M);
    // print_graph(graph, N);

    PathInfo pathInfo = find_shortest_path_by_dijkstra(graph, S, F);
    print_path_info(&pathInfo, stdout);

    destroy_graph(graph);
    deallocate_path_info(&pathInfo);
  }

  return EXIT_SUCCESS;
}
