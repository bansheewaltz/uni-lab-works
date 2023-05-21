#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF INT_MAX
#define uint unsigned int

typedef enum
{
  NO_ERROR,
  BAD_NUMBER_OF_LINES,
  BAD_VERTEX,
  BAD_LENGTH,
  BAD_NUMBER_OF_VERTICES,
  BAD_NUMBER_OF_EDGES,
  NO_SPANNING_TREE,
  ONE_VERTEX,
  MEMORY_ALLOCATION
} ErrorCodes;

void PrintErrorMessage(ErrorCodes errorCode)
{
  switch (errorCode) {
    case BAD_NUMBER_OF_LINES:
      printf("bad number of lines");
      break;
    case BAD_VERTEX:
      printf("bad vertex");
      break;
    case BAD_LENGTH:
      printf("bad length");
      break;
    case BAD_NUMBER_OF_VERTICES:
      printf("bad number of vertices");
      break;
    case BAD_NUMBER_OF_EDGES:
      printf("bad number of edges");
      break;
    case NO_SPANNING_TREE:
      printf("no spanning tree");
      break;
    case MEMORY_ALLOCATION:
      printf("memory allocation errorCode");
      break;
    case ONE_VERTEX:
    case NO_ERROR:
      break;
  }
}

int main()
{
}
