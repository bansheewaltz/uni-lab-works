#include "error.h"

#include <stdbool.h>
#include <stdio.h>

void print_error_message(ReturnCode returnCode)
{
  switch (returnCode) {
    case BAD_NUMBER_OF_LINES:
      puts("bad number of lines");
      break;
    case BAD_VERTEX:
      puts("bad vertex");
      break;
    case BAD_LENGTH:
      puts("bad length");
      break;
    case BAD_NUMBER_OF_VERTICES:
      puts("bad number of vertices");
      break;
    case BAD_NUMBER_OF_EDGES:
      puts("bad number of edges");
      break;
    case NO_SPANNING_TREE:
      puts("no spanning tree");
      break;
    case MEMORY_ALLOCATION_FAIL:
      puts("memory allocation fail");
      break;
    case ONE_VERTEX:
    case SUCCESS:
      break;
  }
}

bool is_error(ReturnCode result)
{
  if (result == SUCCESS) {
    return false;
  }
  return true;
}
