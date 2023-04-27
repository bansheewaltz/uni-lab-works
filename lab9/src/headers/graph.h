#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdint.h>

#define ADJACENCY_LIST 1
#define ADJACENCY_MATRIX 0
#define DIRECTED 1
#define UNDIRECTED 0
#define DENSE_COEFFICIENT 0.5
#define INFINITY_LENGTH UINT64_MAX
#define UNDEFINED -1

typedef struct AdjListNode AdjListNode;

typedef struct {
  int n_vertices;
  int n_edges;
  AdjListNode **adj_lists;
  int *adj_matrix;
  bool representation;
  bool directivity;
} Graph;

typedef struct {
  int src;
  int dst;
  int weight;
} Edge;

void reverse_edge(Edge *edge);
bool is_graph_dense(int n_vertices, int n_edges);
Graph *create_graph(int N, int M, bool directivity);
void destroy_graph(Graph *graph);
void print_graph(Graph *graph);

#endif  // GRAPH_H