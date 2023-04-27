#ifndef INPUT_H
#define INPUT_H

#include "graph.h"

int scan_validate_edge(Edge *edge, int n_vertices);
bool scan_validate_parameters(int *V, int *src, int *dst, int *E);

#endif  // INPUT_H
