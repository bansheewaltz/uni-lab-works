#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.h"
#include "utils.h"

typedef struct {
  int total_value;
  int* picked_weight;
  int* picked_value;
} KnapSackInfo;

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

void find_subset(int width, int table[][width], int i, int j, int weights[],
                 int values[], bool includes[]) {
  if (table[i][j] == 0) {
    return;
  }
  if (table[i - 1][j] == table[i][j]) {
    find_subset(width, table, i - 1, j, weights, values, includes);
  } else {
    find_subset(width, table, i - 1, j - weights[i - 1], weights, values,
                includes);
    // ans.push(i);
    // printf("%d %d\n", weights[i - 1], values[i - 1]);
    includes[i - 1] = true;
  }
}

bool* knapsack(int knapsack_capacity, int weights[], int values[],
               int objects_count) {
  assert(objects_count >= 0);
  assert(knapsack_capacity >= 0);

  int table[objects_count + 1][knapsack_capacity + 1];

  for (int i = 0; i <= objects_count; ++i) {
    for (int j = 0; j <= knapsack_capacity; ++j) {
      if (i == 0 || j == 0) {
        table[i][j] = 0;
      } else if (weights[i - 1] <= j) {
        table[i][j] = max(values[i - 1] + table[i - 1][j - weights[i - 1]],
                          table[i - 1][j]);
      } else {
        table[i][j] = table[i - 1][j];
      }
      // print_table(knapsack_capacity + 1, table, objects_count, values,
      // weights);
    }
  }

#ifdef DEBUG
  // print_table(knapsack_capacity + 1, table, objects_count, values, weights);
#endif

  bool* includes = (bool*)calloc(objects_count, sizeof(bool));
  if_fail(includes == NULL, __FILE__, __LINE__);
  int total_value = table[objects_count][knapsack_capacity];

  find_subset(knapsack_capacity + 1, table, objects_count, knapsack_capacity,
              weights, values, includes);
#ifdef DEBUG
  puts("knapsack includes");
  array_bool_print(includes, objects_count, 3);
#endif
  printf("%d\n", total_value);

  return includes;
}

int main(void) {
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  int objects_count = 0;
  int knapsack_capacity = 0;

  scanf("%d %d", &objects_count, &knapsack_capacity);

  int* weights = (int*)malloc(objects_count * sizeof(int));
  int* values = (int*)malloc(objects_count * sizeof(int));
  if_fail(weights == NULL, __FILE__, __LINE__);
  if_fail(values == NULL, __FILE__, __LINE__);

  for (int i = 0; i < objects_count; ++i) {
    scanf("%d %d", &weights[i], &values[i]);
  }

#ifdef DEBUG
  int* indices = create_indices_array(objects_count);
  if_fail(indices == NULL, __FILE__, __LINE__);
  int alignment = array_int_print_alignment(weights, objects_count);
  puts("original order");
  array_int_print(indices, objects_count, alignment);
  array_int_print(weights, objects_count, alignment);
  array_int_print(values, objects_count, alignment);
#endif
  int* mapping = array_int_radix_sort(weights, objects_count, ASCENDING);
#ifdef DEBUG
  puts("sorted order");
  array_int_print(mapping, objects_count, alignment);
  array_int_print(weights, objects_count, alignment);
  array_int_reorder(values, mapping, objects_count);
  array_int_print(values, objects_count, alignment);
#endif

  knapsack(knapsack_capacity, weights, values, objects_count);
  // for (int i = 0; i < objects_count; ++i) {
  //   if ()
  // }

  free(mapping);
  free(weights);
  free(values);
  return EXIT_SUCCESS;
}
