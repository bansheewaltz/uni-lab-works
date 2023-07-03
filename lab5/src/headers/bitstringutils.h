#ifndef BITSTRINGUTILS_H_
#define BITSTRINGUTILS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

unsigned char bit_to_char(int bit);
bool char_to_bit(int bit_ascii);
bool readbit_ascii(FILE *input);
uint64_t read_bits_ascii(FILE *input, size_t bytes_count);
void fill_last_byte(size_t *bits_printed, FILE *output);
void print_byte_in_bitstring(char byte, bool new_line_flag, FILE *output);
void print_char_in_bitstring(char character, bool new_line_flag, FILE *output);
void print_bits(void *ptr, size_t bytes_count, FILE *output);

#endif  // BITSTRINGUTILS_H_
