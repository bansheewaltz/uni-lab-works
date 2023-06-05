#include "output.h"

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "bitstringutils.h"
#include "main.h"

void array_int_print(int array[], int len, FILE *output)
{
  for (int i = 0; i < len; ++i) {
    fprintf(output, "%d", array[i]);
  }
  fprintf(output, "\n");
}

void array_char_print(uchar array[], int len, FILE *output)
{
  for (int i = 0; i < len; ++i) {
    fprintf(output, "%d ", array[i]);
  }
  fprintf(output, "\n");
}

void print_code_array(CharInfo *charInfo, FILE *output)
{
  for (size_t i = 0; i < charInfo->code_len; ++i) {
    fprintf(output, "%" PRIu8, charInfo->huffman_code[i]);
  }
}

void print_code(CharInfo *charInfo, FILE *output)
{
  fprintf(output, "%d : ", charInfo->character);
  for (size_t i = 0; i < charInfo->code_len; ++i) {
    fprintf(output, "%" PRIu8, charInfo->huffman_code[i]);
  }
  fprintf(output, "\n");
}

void print_codes_lexicographically(CharInfo **chars_info_array,
                                   size_t alphabet_size, FILE *output)
{
  for (size_t i = 0; i < alphabet_size; ++i) {
    print_code(chars_info_array[i], output);
  }
}

void print_coding_stats(CharInfo **chars_info_array, size_t alphabet_size,
                        FILE *output)
{
  int file_size = 0;
  int code_length = 0;
  double information_capacity = 0;

  for (size_t i = 0; i < alphabet_size; ++i) {
    file_size += chars_info_array[i]->freq;
    code_length += chars_info_array[i]->code_len * chars_info_array[i]->freq;
  }
  for (size_t i = 0; i < alphabet_size; ++i) {
    information_capacity +=
        (double)chars_info_array[i]->freq / file_size *
        log2((double)file_size / chars_info_array[i]->freq);
  }
  double redundancy =
      1.0 - (double)file_size / code_length * information_capacity;

  fprintf(output, "Information capacity = %.6f\n", information_capacity);
  fprintf(output, "Input file size = %d\n", file_size);
  fprintf(output, "Code length = %d\n", code_length);
  fprintf(output, "Redundancy = %.6f\n", redundancy);
}
