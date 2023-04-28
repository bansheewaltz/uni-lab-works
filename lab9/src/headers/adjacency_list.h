#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include "graph.h"

struct AdjListNode {
  int dst;
  int length;
  AdjListNode *next;
};

void deallocate_adj_list(Graph *graph);
AdjListNode **create_graph_adj_lists(int n_vertices, int n_edges,
                                     bool directivity);
void print_adj_list_graph(Graph *graph);

#endif  // ADJACENCY_LIST_H
