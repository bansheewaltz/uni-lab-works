#include <stdio.h>
#include <stdlib.h>

<<<<<<< Updated upstream
int main(void) {
  int a = 0;
  return EXIT_SUCCESS;
=======
int int_read(int *n) { return scanf("%d", n); }
int in_bounds(int *n, int a, int b) { return a <= *n && *n <= b; }
int read_number_of_elements(int *n) {
  return int_read(n) && in_bounds(n, 1, 10);
}

int main(void) {
  int number_of_elements = 0;
  if (!read_number_of_elements(&number_of_elements)) printf("bad input");
  return 0;
>>>>>>> Stashed changes
}
