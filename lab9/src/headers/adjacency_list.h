#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include "graph.h"

struct AdjListNode {
  int dst;
  int length;
  AdjListNode *next;
};

void add_adj_list_node(AdjListNode **adj_lists, Edge *edge);
void initialise_lists(AdjListNode *adj_lists[], int N);
void deallocate_adj_list(Graph *graph);
AdjListNode **create_graph_adj_lists(int n_vertices, int n_edges,
                                     bool directivity);
void print_adj_list_graph(Graph *graph);

#endif  // ADJACENCY_LIST_H
