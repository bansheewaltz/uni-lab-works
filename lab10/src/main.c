#include "andrew.h"
#include "error.h"
#include "vector.h"

#define MAX_POINT_COUNT (int)100000

int main(void)
{
  int count = 0;
  if (scanf("%d", &count) == EOF) {
    bad_number_of_lines_error();
  }

  if (count < 0 || count > MAX_POINT_COUNT) {
    bad_number_of_points_error();
  }

  TVector vector = input_vector(count, sizeof(TPoint), scan_point);
  andrew_algorithm(vector);
  destroy_vector(&vector);

  return EXIT_SUCCESS;
}
