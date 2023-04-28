#include "adjacency_list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "input.h"
#include "utils.h"

void add_adj_list_node(AdjListNode **adj_lists, Edge *edge) {
  assert(adj_lists != NULL);

  AdjListNode *new_node = (AdjListNode *)malloc(sizeof(AdjListNode));
  if_fail(new_node == NULL, __FILE__, __LINE__ - 1);

  new_node->dst = edge->dst;
  new_node->length = edge->weight;
  new_node->next = adj_lists[edge->src];

  adj_lists[edge->src] = new_node;
}

void initialise_lists(AdjListNode *adj_lists[], int V) {
  assert(adj_lists != NULL);

  for (int i = 0; i < V; i++) {
    adj_lists[i] = NULL;
  }
}

void deallocate_adj_list(Graph *graph) {
  assert(graph != NULL);

  for (int i = 0; i < graph->n_vertices + 1; ++i) {
    AdjListNode *list_head = graph->adj_lists[i];

    if (list_head != NULL) {
      AdjListNode *tmp_next = list_head->next;

      while (tmp_next != NULL) {
        free(list_head);
        list_head = tmp_next;
        tmp_next = list_head->next;
      }
      free(list_head);
    }
  }

  free(graph->adj_lists);
}

AdjListNode **create_graph_adj_lists(int n_vertices, int n_edges,
                                     bool directivity) {
  AdjListNode **adj_lists;  // V + 1 b/c idx 0 node will be ingored
  adj_lists = (AdjListNode **)malloc((n_vertices + 1) * sizeof(AdjListNode *));
  if_fail(adj_lists == NULL, __FILE__, __LINE__ - 1);

  initialise_lists(adj_lists, n_vertices + 1);

  for (int i = 0; i < n_edges; ++i) {
    Edge edge;

    bool ret = scan_validate_edge(&edge, n_vertices);
    if (ret != SUCCESS) {
      print_error_terminate("bad number of lines");
    }

    add_adj_list_node(adj_lists, &edge);
    if (directivity == UNDIRECTED) {
      reverse_edge(&edge);
      add_adj_list_node(adj_lists, &edge);
    }
  }

  return adj_lists;
}

void print_adj_list_graph(Graph *graph) {
  assert(graph != NULL);
  assert(graph->adj_lists != NULL);

  for (int i = 1; i <= graph->n_vertices; ++i) {
    AdjListNode *ptr = graph->adj_lists[i];

    while (ptr != NULL) {
      printf("%d —> %d (%d)\t", i, ptr->dst, ptr->length);
      ptr = ptr->next;
    }

    printf("\n");
  }
}
