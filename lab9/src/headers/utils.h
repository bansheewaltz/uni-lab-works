#ifndef UTILS_H
#define UTILS_H

#include "typedefs.h"

#define SUCCESS 1
#define FAILURE 0
#define INFINITY UINT64_MAX
#define UNDEFINED -1
#define ADJACENCY_LIST 1
#define ADJACENCY_MATRIX 0
#define DIRECTED 1
#define UNDIRECTED 0
#define DENSE_COEFFICIENT 0.5

void print_error_terminate(char message[]);
int get_nth_triangular_number(int number);

void print_adj_matrix_graph(Graph *graph);
void print_adj_list_graph(Graph *graph);
void print_graph(Graph *graph);

#endif  // UTILS_H
