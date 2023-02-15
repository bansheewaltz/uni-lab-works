// Topological sort of unlabeled undirected weighted graph
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define SUCCESS 1
#define FAILURE 0
#define CLOSED_INTERVAL(l, value, r) (l <= value && value <= r)
/* changable variables */
#define MAX_VERTICES_NUM 5000
#define MAX_EDGES_NUM (N * (N - 1) / 2)
#define MAX_EDGE_LEN INT_MAX

struct Graph {
  int num_of_vertices;
  int num_of_edges;
  struct Node **adjacency_lists;
};

struct Node {  // adjacency list node
  int dst;
  int length;
  struct Node *next;
};

struct Edge {
  int src;
  int dst;
  int length;
};

void print_error_terminate(char message[]) {
  puts(message);
  exit(EXIT_SUCCESS);  // but actually FAILURE
}

void add_node(struct Graph *graph, int src, int dst, int length) {
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  if (new_node == NULL) {
    print_error_terminate("heap buffer overflow");
  }

  new_node->dst = dst;
  new_node->length = length;
  new_node->next = graph->adjacency_lists[src];
  graph->adjacency_lists[src] = new_node;
}

void initialise_lists(struct Node *adjacency_lists[], int N) {
  for (int i = 0; i < N; i++) {
    adjacency_lists[i] = NULL;
  }
}

struct Graph *create_graph(struct Edge *edges[], int N, int M) {
  struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
  graph->num_of_vertices = N;
  graph->num_of_edges = M;
  graph->adjacency_lists = (struct Node **)malloc(N * sizeof(struct Node *));
  initialise_lists(graph->adjacency_lists, N);

  for (int i = 0; i < M; ++i) {
    int src = (*edges)[i].src;
    int dst = (*edges)[i].dst;
    int length = (*edges)[i].length;

    add_node(graph, src, dst, length);
    add_node(graph, dst, src, length);
  }

  return graph;
}

void print_graph(struct Graph *graph, int M) {
  for (int i = 0; i < M; i++) {
    struct Node *ptr = graph->adjacency_lists[i];
    while (ptr != NULL) {
      printf("%d —> %d (%d)\t", i, ptr->dst, ptr->length);
      ptr = ptr->next;
    }

    printf("\n");
  }
}

bool scan_num_of_vertices(int *N) {
  if (scanf("%i", N) && CLOSED_INTERVAL(0, *N, MAX_VERTICES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of vertices");
  return false;
}

bool scan_src_dst(int *S, int *F, int N) {
  if ((scanf("%d", S) && CLOSED_INTERVAL(1, *S, N)) &&
      (scanf("%d", F) && CLOSED_INTERVAL(1, *F, N))) {
    return true;
  }

  print_error_terminate("bad vertex");
  return false;
}

bool scan_num_of_edges(int *M, int N) {
  if (scanf("%d", M) && CLOSED_INTERVAL(0, *M, MAX_EDGES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of edges");
  return false;
}

bool scan_edge_len(int *length) {
  if (scanf("%d", length) && CLOSED_INTERVAL(0, *length, MAX_EDGE_LEN)) {
    return true;
  }

  print_error_terminate("bad length");
  return false;
}

int scan_edge(int *src, int *dst, int N, int *length) {
  if (scan_src_dst(src, dst, N) && scan_edge_len(length)) {
    return SUCCESS;
  }

  return FAILURE;
}

bool validate_parameters(int *N, int *S, int *F, int *M) {
  return scan_num_of_vertices(N) &&  //
         scan_src_dst(S, F, *N) &&   //
         scan_num_of_edges(M, *N);
}

bool validate_input(int *N, int *S, int *F, int *M, struct Edge ***edges) {
  if (!validate_parameters(N, S, F, M)) {
    return FAILURE;
  }

  *edges = (struct Edge **)malloc(sizeof(struct Edge *) * (*M));
  if (*edges == NULL) {
    print_error_terminate("heap buffer overflow");
  }

  bool ret = SUCCESS;
  int i;

  for (i = 0; i < *M && ret == SUCCESS; ++i) {
    ret = scan_edge(&(*edges)[i]->src, &(*edges)[i]->dst, *N,
                    &(*edges)[i]->length);
  }
  if (i != *M || ret != SUCCESS) {
    print_error_terminate("bad number of lines");
  }

  return SUCCESS;
}

int main(void) {
  int N;  // number of vertices
  int S;  // source
  int F;  // destination
  int M;  // number of edges
  struct Edge **edges = NULL;

  if (validate_input(&N, &S, &F, &M, &edges) == SUCCESS) {
    struct Graph *graph = create_graph(edges, N, M);

    print_graph(graph, M);
  }

  return EXIT_SUCCESS;
}
