#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.h"
#include "utils.h"

typedef struct {
  int total_value;
  int *picked_weight;
  int *picked_value;
} KnapSackInfo;

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

void find_subset(int width, int table[][width], int i, int j, int weights[],
                 int values[]) {
  if (table[i][j] == 0) {
    return;
  }
  if (table[i - 1][j] == table[i][j]) {
    find_subset(width, table, i - 1, j, weights, values);
  } else {
    find_subset(width, table, i - 1, j - weights[i], weights, values);
    // ans.push(i);
    printf("%d, %d\n", weights[i], values[i]);
  }
}

void print_table(int width, int table[][width], int objects_count, int values[],
                 int weights[]) {
  for (int i = -3; i <= width; ++i) {
    if (i < 0) {
      printf("%4s ", "");
    } else {
      printf("%4d ", i);
    }
  }
  printf("\n");

  for (int i = 0; i <= objects_count; ++i) {
    for (int j = -3; j <= width; ++j) {
      if (i == 0 && j < 0) {
        if (j == -3) {
          printf("%4s ", "val");
        }
        if (j == -2) {
          printf("%4s ", "wgt");
        }
        if (j == -1) {
          printf("%4s ", "obj");
        }
        continue;
      }
      if (j == -3) {
        printf("%4d ", values[i - 1]);
      }
      if (j == -2) {
        printf("%4d ", weights[i - 1]);
      }
      if (j == -1) {
        printf("%4d ", i);
      }
      if (j >= 0) {
        printf("%4d ", table[i][j]);
      }
    }
    printf("\n");
  }
}

void knapsack(int knapsack_capacity, int weights[], int values[],
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
    }
  }

#ifdef DEBUG
  print_table(knapsack_capacity, table, objects_count, values, weights);
#endif

  bool *include = (bool *)calloc(objects_count, sizeof(bool));
  if_fail(include == NULL, __FILE__, __LINE__);
  // int total_value = table[objects_count][knapsack_capacity];

  // for (int i = objects_count - 1; i >= 0; --i) {
  //   for (int j = knapsack_capacity; j >= 0; --j) {
  //     if (table[i][j] != table[i + 1][j])
  //   }
  // }
  // if (table[objects_count][knapsack_capacity] !=
  //     table[objects_count + 1][knapsack_capacity]) {
  //   include[objects_count] = 1;
  // }

  // printf("%d\n", total_value);
  // find_subset(knapsack_capacity + 1, table, objects_count, knapsack_capacity,
  //             weights, values);
}

int main(void) {
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  int objects_count;
  int knapsack_capacity;

  scanf("%d %d", &objects_count, &knapsack_capacity);

  int *weights = (int *)malloc(objects_count * sizeof(int));
  int *values = (int *)malloc(objects_count * sizeof(int));
  if_fail(weights == NULL, __FILE__, __LINE__);
  if_fail(values == NULL, __FILE__, __LINE__);

  for (int i = 0; i < objects_count; ++i) {
    scanf("%d %d", &weights[i], &values[i]);
  }
  radix_sort_int_int(values, weights, objects_count, ASCENDING);
  print_int_array(weights, objects_count);
  print_int_array(values, objects_count);

  // printf("The solution is : %d",
  //        knapsack(knapsack_capacity, weights, values, objects_count));

  knapsack(knapsack_capacity, weights, values, objects_count);

  free(weights);
  free(values);
  return EXIT_SUCCESS;
}
