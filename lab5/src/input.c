#include "input.h"

#include <assert.h>
#include <stdlib.h>

#include "main.h"
#include "utils.h"

void scan_chars_frequencies_from_input(CharInfo **dictionary, FILE *input)
{
  // initialize info for every char in an alphabet to minimize number of
  // actions during iteration over input
  for (int i = 0; i < CHARSET_SIZE; ++i) {
    dictionary[i] = create_and_initialize_char_info(i);
  }

  uchar buffer[BUFFER_SIZE];

  size_t chars_read = 0;
  while ((chars_read = fread(buffer, sizeof(uchar), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < chars_read; ++i) {
      ++dictionary[buffer[i]]->freq;
    }
  }

  // free info for chars that are never encountered
  for (int i = 0; i < CHARSET_SIZE; ++i) {
    if (dictionary[i]->freq == 0) {
      free(dictionary[i]);
      dictionary[i] = NULL;
    }
  }
}

int read_alphabet_size_checked(FILE *input)
{
  byte alph_size = 0;
  size_t rc = fread(&alph_size, sizeof(uchar), 1, input);
  assert(rc == 1);
  if (alph_size == 0b00000000) {
    return EIGHT_BIT_CHARACTER_SET_SIZE;
  }
  return (int)alph_size;
}

size_t get_alphabet_size(CharInfo **dictionary)
{
  size_t alph_size = 0;
  for (size_t i = 0; i < CHARSET_SIZE; ++i) {
    if (dictionary[i] != NULL) {
      ++alph_size;
    }
  }
  return alph_size;
}
