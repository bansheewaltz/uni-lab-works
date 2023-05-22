#include "error.h"

#include <stdbool.h>
#include <stdio.h>

void print_error_message(ReturnCode returnCode)
{
  switch (returnCode) {
    case E_BAD_NUMBER_OF_LINES:
      puts("bad number of lines");
      break;
    case E_BAD_VERTEX:
      puts("bad vertex");
      break;
    case E_BAD_LENGTH:
      puts("bad length");
      break;
    case E_BAD_NUMBER_OF_VERTICES:
      puts("bad number of vertices");
      break;
    case E_BAD_NUMBER_OF_EDGES:
      puts("bad number of edges");
      break;
    case E_NO_SPANNING_TREE:
      puts("no spanning tree");
      break;
    case E_MEMORY_ALLOCATION_FAIL:
      puts("memory allocation fail");
      break;
    case E_ONE_VERTEX:
    case E_SUCCESS:
      break;
  }
}

bool is_error(ReturnCode result)
{
  if (result == E_SUCCESS) {
    return false;
  }
  return true;
}
