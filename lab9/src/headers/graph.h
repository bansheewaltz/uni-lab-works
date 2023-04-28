#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdint.h>

#define ADJACENCY_LIST 1
#define ADJACENCY_MATRIX 0
#define DIRECTED 1
#define UNDIRECTED 0
#define DENSITY_CONSTANT 0.3

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
bool is_graph_dense(int V, int E, bool directivity);
Graph *create_graph(int V, int E, bool directivity);
void destroy_graph(Graph *graph);
void print_graph(Graph *graph);

#endif  // GRAPH_H
