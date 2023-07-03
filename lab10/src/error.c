#include "error.h"

void bad_number_of_points_error(void)
{
  printf("bad number of points");
  exit(EXIT_SUCCESS);
}

void bad_number_of_lines_error(void)
{
  printf("bad number of lines");
  exit(EXIT_SUCCESS);
}

void other_error(char* file, int line)
{
  printf("other error (%s, %d)", file, line);
  exit(EXIT_SUCCESS);
}
