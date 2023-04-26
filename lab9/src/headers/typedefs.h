#ifndef typedefs_H
#define typedefs_H

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>

/* changable variables */
#define MAX_VERTICES_NUM 5000
#define MAX_EDGES_NUM (N * (N - 1) / 2)
#define MAX_EDGE_LEN INT_MAX

typedef struct Graph Graph;
typedef struct Edge Edge;
typedef struct AdjListNode AdjListNode;
typedef struct PathInfo PathInfo;

struct Graph {
  int n_vertices;
  int n_edges;
  AdjListNode **adj_lists;
  int *adj_matrix;
  bool representation;
  bool directivity;
};

struct Edge {
  int src;
  int dst;
  int weight;
};

struct AdjListNode {
  int dst;
  int length;
  AdjListNode *next;
};

struct PathInfo {
  int src;
  int dst;
  uint64_t *distances;
  int *previous_arr;
};

#endif  // typedefs_H
