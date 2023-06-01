#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char bit_to_char(int bit)
{
  if (bit == 0)
    return '0';
  if (bit == 1)
    return '1';

  assert(false);
  return false;
}

bool char_to_bit(int bit_ascii)
{
  if (bit_ascii == '0')
    return 0;
  if (bit_ascii == '1')
    return 1;

  assert(false);
  return false;
}

bool readbit_ascii(FILE *input)
{
  int bit_ascii = getc(input);
  assert(bit_ascii != -1);
  return char_to_bit(bit_ascii);
}

uint64_t read_bits_ascii(FILE *input, size_t bytes_count)
{
  assert(bytes_count <= sizeof(int64_t));

  uint64_t value = 0x0U;
  uint64_t power_of_2 = 1;
  size_t bits_count = bytes_count * CHAR_BIT;

  unsigned char *buffer = malloc(bits_count);
  size_t rc = fread(buffer, sizeof(uint8_t), bits_count, input);
  assert(rc == bits_count);

  for (size_t i = bits_count - 1; i < bits_count; --i) {
    bool bit = char_to_bit(buffer[i]);
    if (bit == 1) {
      value += power_of_2;
    }
    power_of_2 *= 2;
  }

  free(buffer);
  return value;
}

void fill_last_byte(size_t *bits_printed, FILE *output)
{
  if (*bits_printed % CHAR_BIT == 0)
    return;

  while (*bits_printed % CHAR_BIT != 0) {
    int rc = putc('0', output);
    assert(rc != -1);
    *bits_printed += 1;
  }
}

void print_byte_in_bitstring(char byte, bool new_line_flag, FILE *output)
{
  int rc = 0;
  for (size_t i = CHAR_BIT - 1; i >= 0; --i) {
    rc = fprintf(output, "%d", ((unsigned int)byte >> i) & 1U ? 1 : 0);
  }
  assert(rc != -1);
  if (new_line_flag == true) {
    rc = fprintf(output, "\n");
  }
  assert(rc != -1);
}

void print_char_in_bitstring(char character, bool new_line_flag, FILE *output)
{
  print_byte_in_bitstring(character, new_line_flag, output);
}

void print_bits(void *ptr, size_t bytes_count, FILE *output)
{
  uint8_t *byte_array = (uint8_t *)ptr;

  for (size_t i = bytes_count - 1; i < bytes_count; --i) {
    for (unsigned int j = CHAR_BIT - 1; j < CHAR_BIT; --j) {
      unsigned int bit = ((unsigned int)byte_array[i] >> j) & 0x1U;
      int rc = fprintf(output, "%" PRIu8, (uint8_t)bit);
      assert(rc != -1);
    }
  }
}
