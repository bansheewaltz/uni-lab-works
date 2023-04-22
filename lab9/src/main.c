// Dijkstra algorithm for undirected weighted graph
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
#define CLOSED_INTERVAL(l, value, r) (l <= value && value <= r)

typedef struct Graph Graph;
typedef struct AdjListNode AdjListNode;

struct Graph {
  int num_of_vertices;
  int num_of_edges;
  AdjListNode **adjacency_lists;
};

struct AdjListNode {
  int dst;
  int length;
  AdjListNode *next;
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

void add_node(Graph *graph, int src, int dst, int length) {
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
  for (int i = 0; i < N; i++) {
    adjacency_lists[i] = NULL;
  }
}

Graph *create_graph(struct Edge *edges[], int N, int M) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->num_of_vertices = N;
  graph->num_of_edges = M;
  graph->adjacency_lists =  // N + 1 because 0 node will be ingored
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
  for (int i = 1; i < M + 1; i++) {
    AdjListNode *ptr = graph->adjacency_lists[i];
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

bool scan_path(int *S, int *F, int N) {
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
  if (scan_path(src, dst, N) && scan_edge_len(length)) {
    return SUCCESS;
  }

  return FAILURE;
}

bool validate_parameters(int *N, int *S, int *F, int *M) {
  return scan_num_of_vertices(N) &&  //
         scan_path(S, F, *N) &&      //
         scan_num_of_edges(M, *N);   //
}

bool validate_input(int *N, int *S, int *F, int *M, struct Edge ***edges) {
  if (!validate_parameters(N, S, F, M)) {
    return FAILURE;
  }

  *edges = (struct Edge **)malloc(sizeof(struct Edge *) * (*M));
  if (*edges == NULL) {
    print_error_terminate("heap buffer overflow");
  }

  for (int i = 0; i < *M; ++i) {
    (*edges)[i] = (struct Edge *)malloc(sizeof(struct Edge));
    bool ret = scan_edge(&(*edges)[i]->src, &(*edges)[i]->dst, *N,
                         &(*edges)[i]->length);
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
  for (int i = 0; i < M; ++i) {
    free(edges[i]);
  }
  free(edges);
}

int main(void) {
  int N;  // number of vertices
  int S;  // source
  int F;  // destination
  int M;  // number of edges
  // the graph is not being built until all the input data is checked
  struct Edge **edges_temporary_storage = NULL;

  if (validate_input(&N, &S, &F, &M, &edges_temporary_storage) == SUCCESS) {
    Graph *graph = create_graph(edges_temporary_storage, N, M);
    deallocate_temporary_storage(edges_temporary_storage, M);

    print_graph(graph, N);
    destroy_graph(graph);
  }

  return EXIT_SUCCESS;
}
