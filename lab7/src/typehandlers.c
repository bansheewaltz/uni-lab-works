#include <stddef.h>
#include <stdio.h>

#include "main.h"
#include "utils.h"

Stack *stack_init(int capacity)
{
  Stack *stack = malloc(sizeof(Stack));
  int *stack_array = malloc(sizeof(int) * (size_t)capacity);
  if (stack != NULL && stack_array != NULL) {
    stack->stack_array = stack_array;
    stack->stack_top = -1;
    stack->stack_capacity = capacity;
  }
  return stack;
}

void stack_free(Stack *stack)
{
  if (stack == NULL) {
    return;
  }
  if (stack->stack_array != NULL) {
    free(stack->stack_array);
  }
  free(stack);
}

bool stack_push(Stack *stack, int element)
{
  if (stack == NULL || stack->stack_array == NULL) {
    return FAILURE;
  }
  if (stack->stack_capacity == stack->stack_top + 1) {
    return FAILURE;
  }
  stack->stack_array[++stack->stack_top] = element;
  return SUCCESS;
}

void stack_print(Stack *stack)
{
  if (stack == NULL || stack->stack_array == NULL) {
    return;
  }
  int *stack_array = stack->stack_array;
  for (int i = stack->stack_top; i >= 0; --i) {
    printf("%d%s", stack_array[i], i == 0 ? "\n" : " ");
  }
}

Graph *graph_init(int vertices_count, int edges_count)
{
  Graph *graph = malloc(sizeof(Graph));
  size_t graph_matrix_size = (size_t)vertices_count * (size_t)vertices_count;
  bool *graph_array = calloc(graph_matrix_size, sizeof(bool));

  if (graph != NULL && graph_array != NULL) {
    graph->vertices_count = vertices_count;
    graph->edges_count = edges_count;
    graph->graph_array = graph_array;
  }

  return graph;
}

void graph_free(Graph *graph)
{
  if (graph == NULL) {
    return;
  }
  if (graph->graph_array != NULL) {
    free(graph->graph_array);
  }
  free(graph);
}
