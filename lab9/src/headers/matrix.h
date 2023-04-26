#ifndef MATRIX_H
#define MATRIX_H

#include "typedefs.h"

int *create_graph_adj_matrix(Graph *graph);
int get_adj_matrix_entry_idx(Graph *graph, int row, int column);
int get_adj_matrix_entry(Graph *graph, int row, int column);
bool is_matrix_neighbour(Graph *graph, int row, int column);

#endif  // MATRIX_H
