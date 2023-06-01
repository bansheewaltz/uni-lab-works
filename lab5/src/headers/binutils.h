#ifndef BITUTILS_H_
#define BITUTILS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "main.h"

void set_bit(uchar *byte, bool bit, int position);
void reset_byte(uchar *byte, int *bit_position);
bool get_bit(uchar byte, int position);
bool readbit_buffered(FILE *input);
void serialize_char(uchar character, FILE *output);
void writebit_buffered(int bit, bool flush_flag, FILE *output);

#endif  // BITUTILS_H_
