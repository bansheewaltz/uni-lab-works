#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "vector.h"

typedef struct {
  int *array;
  size_t rows;
  size_t columns;
} Table;

Table *create_table(int objects_count, int max_weight) {
  Table *table = (Table *)malloc(sizeof(Table));

  table->rows = objects_count + 1;
  table->columns = max_weight + 1;
  table->array = (int *)malloc(table->rows * table->columns * sizeof(int));

  return table;
}

void set_table_value(Table *table, int row, int column, int value) {
  table->array[row * column + column] = value;
}

void fill_table(Table *table, int *objects, int *weights) {
  for (int i = 0; i < table->rows; ++i) {
    set_table_value(table, 0, i, 0);
  }
  for (int i = 0; i < table->columns; ++i) {
    set_table_value(table, i, 0, 0);
  }

  for (int object_number = 1; object_number <= table->rows; ++object_number) {
    for
  }
}

int find_max(int *array, size_t array_size) {
  assert(array != NULL);
  int temp_max = array[1];

  for (size_t i = 1; i < array_size; ++i) {
    if (array[i] > temp_max) {
      temp_max = array[i];
    }
  }

  return temp_max;
}

int main(void) {
  int objects_count;
  int knapsack_capacity;

  scanf("%d %d", &objects_count, &knapsack_capacity);

  int *objects = (int *)malloc(objects_count * sizeof(int));
  int *weights = (int *)malloc(objects_count * sizeof(int));
  if_fail(objects == NULL, __FILE__, __LINE__ - 2);
  if_fail(weights == NULL, __FILE__, __LINE__ - 2);

  for (int i = 0; i < objects_count; ++i) {
    scanf("%d %d", &objects[i], &weights[i]);
  }

  int max_weight = find_max(weights, objects_count);

  Table *table = create_table(objects_count, max_weight);
  fill_table(table, objects, weights);

  free(table);

  return EXIT_SUCCESS;
}
