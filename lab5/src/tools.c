#ifndef TOOLS_C_
#define TOOLS_C_

#include <error.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

void check_null_pointer(void *p)
{
  if (p == NULL) {
    perror("archiver error");
    exit(1);
  }
}

uint8_t *array_uint8_duplicate(uint8_t src[], size_t len)
{
  uint8_t *dst = (uint8_t *)malloc(sizeof(uint8_t) * len);

  for (size_t i = 0; i < len; ++i) {
    dst[i] = src[i];
  }

  return dst;
}

#endif  // TOOLS_C_
