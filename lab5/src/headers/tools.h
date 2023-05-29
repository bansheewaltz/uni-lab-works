#ifndef TOOLS_C_
#define TOOLS_C_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void check_null_pointer(void *p);
void open_streams(FILE **input, FILE **output);
uint8_t *array_uint8_duplicate(uint8_t src[], size_t len);

#endif  // TOOLS_C_
