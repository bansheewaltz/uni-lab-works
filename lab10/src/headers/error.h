#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

void bad_number_of_points_error(void);
void bad_number_of_lines_error(void);
void other_error(char* file, int line);

#endif
