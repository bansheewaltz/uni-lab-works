#include "error.h"

#include <stdbool.h>
#include <stdio.h>

void print_error_message(ReturnCode return_code)
{
  switch (return_code) {
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
    default:
      break;
  }
}

void print_error_message_if_any(ReturnCode return_code) {
  print_error_message(return_code);
}

bool is_error(ReturnCode result)
{
  if (result == E_SUCCESS) {
    return false;
  }
  return true;
}
