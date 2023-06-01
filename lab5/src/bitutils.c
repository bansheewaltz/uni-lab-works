#include "bitutils.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "output.h"
#include "typedefs.h"

uchar bit_to_char(int bit)
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

void set_bit(uchar *byte, bool bit, int position)
{
  *byte |= (uint)bit << (CHAR_BIT - (uint)position);
}

void reset_byte(uchar *byte, int *bit_position)
{
  *byte = 0b00000000;
  *bit_position = 1;
}

bool get_bit(uchar byte, int position)
{
  return (byte >> (CHAR_BIT - position)) & 0b00000001;
  // return ((uint)byte >> (CHAR_BIT - (uint)position)) & 0x0001U;
}

bool readbit_buffered(FILE *input)
{
  static int bit_position = 1;  // in byte, from left
  static uchar byte;

  if (bit_position == 1) {
    fread(&byte, sizeof(uchar), 1, input);
  }

  uint8_t bit = get_bit(byte, bit_position++);
  if (bit_position % CHAR_BIT == 1) {
    bit_position = 1;
  }

#ifdef DEBUG
  // printf("%d", bit);
#endif

  return bit;
}

void fill_last_byte(size_t *bits_printed, FILE *output)
{
  if (*bits_printed % 8 != 0) {
    for (; *bits_printed % 8 != 0; ++*bits_printed) {
      putc('0', output);
    }
  }
}

void serialize_char(uchar character, FILE *output)
{
  for (int i = CHAR_BIT - 1; i >= 0; --i) {
    writebit_buffered((character >> i) & 1 ? 1 : 0, CONTINUE, output);
  }
}

void writebit_buffered(int bit, bool flush_flag, FILE *output)
{
  static int bit_position = 1;  // in byte, from left
  static uchar byte = 0b00000000;

  if (flush_flag == FLUSH_BYTE) {
    if (bit_position == 1) {
      return;
    }
    for (; bit_position < CHAR_BIT; ++bit_position) {
      set_bit(&byte, bit, bit_position);
#ifdef DEBUG
      print_byte_in_binary(byte, NEW_LINE, stdout);
      if (bit_position == 8) {
        printf("flush\n");
      }
#endif
    }
    fwrite(&byte, sizeof(uchar), 1, output);
    reset_byte(&byte, &bit_position);
  }

  if (flush_flag == CONTINUE) {
    set_bit(&byte, bit, bit_position);
#ifdef DEBUG
    print_byte_in_binary(byte, NEW_LINE, stdout);
    if (bit_position == 8) {
      printf("flush\n");
    }
#endif
    if (bit_position % CHAR_BIT == 0) {
      fwrite(&byte, sizeof(uchar), 1, output);
      reset_byte(&byte, &bit_position);
    } else {
      ++bit_position;
    }
  }
}

uint64_t read_bits_ascii(FILE *input, size_t bytes_count)
{
  assert(bytes_count <= sizeof(int64_t));

  uint64_t value = 0x0U;
  uint64_t power_of_2 = 1;
  size_t bits_count = bytes_count * CHAR_BIT;

  uchar *buffer = malloc(bits_count);
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

bool readbit_ascii(FILE *input)
{
  int bit_ascii = getc(input);
  assert(bit_ascii != -1);
  return char_to_bit(bit_ascii);
}
