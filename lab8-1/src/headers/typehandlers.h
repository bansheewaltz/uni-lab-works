#ifndef TYPEHANDLERS_H_
#define TYPEHANDLERS_H_

#include "main.h"

Stack *stack_init(int capacity);
void stack_free(Stack *stack);
bool stack_push(Stack *stack, int element);
void stack_print(Stack *stack);

Graph *graph_init(int vertices_count, int edges_count);
void graph_free(Graph *graph);

#endif  // TYPEHANDLERS_H_
