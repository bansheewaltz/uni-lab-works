#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool int_read(int *n) { return scanf("%d", n); }
bool char_read(char *c) { return scanf("%c", c); }
bool in_bounds(int *n, int a, int b) { return a <= *n && *n <= b; }
void print_error(void) { printf("bad input"); }
bool clear_input_buffer(void) {
  int c;

  while ((c = getchar()) != '\n')
    ;
  return true;
}
bool overflow_check(int current_n, int limit) { return current_n > limit; }
bool underflow_check(int current_n, int limit) { return current_n < limit; }
bool read_number_of_elements(int *n) {
  return int_read(n) && in_bounds(n, 1, 10) && clear_input_buffer();
}
void read_elements(int *array);
bool char_is_digit(char input) { return '1' <= input && input <= '9'; }
bool element_is_good(char input) {
  return (char_is_digit(input) || input == ' ');
}
void array_print(int *array, int array_size) {
  for (int i = 0; i < array_size; ++i) {
    printf("%d%s", array[i], i == array_size - 1 ? "" : " ");
  }
}

int main(void) {
  int number_of_elements = 0;
  bool error = false;
  int *array = NULL;
  int sum = 0;

  if (read_number_of_elements(&number_of_elements)) {
    array = malloc(sizeof(int) * number_of_elements);
    char input = 0;
    int i = 0;

    while (char_read(&input) && element_is_good(input) && !error) {
      if (char_is_digit(input)) {
        int number = 0;
        ungetc(input, stdin);
        if (scanf("%d", &number)) {
          array[i++] = number;
          sum += number;
        }

        if (overflow_check(i, number_of_elements)) error = true;
      }
    }

    if (underflow_check(i, number_of_elements)) error = true;
  } else {
    error = true;
  }

  if (error)
    print_error();
  else {
    // array_print(array, number_of_elements);
    // printf("\n");
    if (overflow_check(sum, INT_MAX))
      printf("overflow");
    else
      printf("%d", sum);
  }

  return 0;
}
