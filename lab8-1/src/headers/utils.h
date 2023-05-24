#ifndef TYPEHANDLERS_H_
#define TYPEHANDLERS_H_

#include "main.h"

Graph *graph_init(int vertices_count, int edges_count);
void graph_add_edge(Graph *graph, Edge *edge);
void graph_free(Graph *graph);

#endif  // TYPEHANDLERS_H_
