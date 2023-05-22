#ifndef ERROR_H_
#define ERROR_H_

#include <stdbool.h>

typedef enum
{
  SUCCESS,
  BAD_NUMBER_OF_LINES,
  BAD_VERTEX,
  BAD_LENGTH,
  BAD_NUMBER_OF_VERTICES,
  BAD_NUMBER_OF_EDGES,
  NO_SPANNING_TREE,
  ONE_VERTEX,
  MEMORY_ALLOCATION_FAIL
} ReturnCode;

void print_error_message(ReturnCode returnCode);
bool is_error(ReturnCode result);

#endif  // ERROR_H_
