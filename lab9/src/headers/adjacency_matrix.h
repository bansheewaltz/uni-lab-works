#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include "typedefs.h"

int *create_graph_adj_matrix(Graph *graph);
int get_adj_matrix_entry_idx(Graph *graph, int row, int column);
int get_adj_matrix_entry(Graph *graph, int row, int column);
bool is_matrix_neighbour(Graph *graph, int row, int column);
void print_adj_matrix_graph(Graph *graph);

#endif  // ADJACENCY_MATRIX_H
