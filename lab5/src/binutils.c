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
  *byte = 0x00U;
  *bit_position = 1;
}

bool get_bit(uint8_t byte, unsigned int position)
{
  assert(position <= CHAR_BIT);
  unsigned int shift = (unsigned int)CHAR_BIT - position;
  return (unsigned int)(byte >> shift) & 0x01U;
}

bool readbit_buffered(FILE *input, bool flush_byte_flag)
{
  static unsigned int bit_position = 1;  // in byte, from left
  static uchar byte;

  if (flush_byte_flag == true && bit_position == 1) {
    return 0;
  }

  if (bit_position == 1) {
    size_t rc = fread(&byte, sizeof(uchar), 1, input);
    assert(rc != -1);
  }

  uint8_t bit = get_bit(byte, bit_position++);

  if (bit_position % CHAR_BIT == 1) {
    bit_position = 1;
  }

  if (flush_byte_flag == true) {
    bit_position = 1;
  }

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
  static uint8_t byte = 0x00U;

  if (flush_flag == FLUSH_BYTE) {
    if (bit_position == 1) {
      return;
    }
    for (; bit_position < CHAR_BIT; ++bit_position) {
      set_bit(&byte, bit, bit_position);
    }
    fwrite(&byte, sizeof(uchar), 1, output);
    reset_byte(&byte, &bit_position);
  }

  if (flush_flag == CONTINUE) {
    set_bit(&byte, bit, bit_position);
    if (bit_position % CHAR_BIT == 0) {
      fwrite(&byte, sizeof(uchar), 1, output);
      reset_byte(&byte, &bit_position);
    } else {
      ++bit_position;
    }
  }
}
