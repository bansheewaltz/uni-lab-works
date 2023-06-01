#include "binutils.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "output.h"
#include "typedefs.h"

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
