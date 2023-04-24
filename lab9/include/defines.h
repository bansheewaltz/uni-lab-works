#ifndef DEFINES_H
#define DEFINES_H

#include <inttypes.h>

/* changable variables */
#define MAX_VERTICES_NUM 5000
#define MAX_EDGES_NUM (N * (N - 1) / 2)
#define MAX_EDGE_LEN INT_MAX
/* utility stuff */
#define SUCCESS 1
#define FAILURE 0
#define INFINITY UINT64_MAX
#define UNDEFINED -1
#define ADJACENCY_LIST 1
#define ADJACENCY_MATRIX 0
#define IS_IN_BOUNDS(l, value, r) (l <= value && value <= r)

typedef struct Graph Graph;
typedef struct AdjListNode AdjListNode;
typedef struct PathInfo PathInfo;

struct Graph {
  int n_vertices;
  int n_edges;
  AdjListNode **adjacency_lists;
  int *adjacency_matrix;
  bool representation;
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

#endif  // DEFINES_H
