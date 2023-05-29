#include "input.h"

#include "main.h"
#include "utils.h"

void scan_chars_frequencies_from_file(CharInfo **chars_info, FILE *input)
{
  uchar buffer[BUFFER_SIZE];
  size_t chars_read;

  while ((chars_read = fread(buffer, sizeof(uchar), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < chars_read; ++i) {
      if (chars_info[buffer[i]] == NULL) {
        chars_info[buffer[i]] = create_and_initialize_char_info(buffer[i]);
      }
      ++chars_info[buffer[i]]->freq;
    }
  }
}

int read_alphabet_size(FILE *input)
{
  byte alph_size;
  fread(&alph_size, sizeof(uchar), 1, input);

  if (alph_size == 0b00000000) {
    return EIGHT_BIT_CHARACTER_SET_SIZE;
  }
  return (int)alph_size;
}

size_t get_alphabet_size(CharInfo **chars_info)
{
  size_t alph_size = 0;

  for (size_t i = 0; i < CHARSET_SIZE; ++i) {
    if (chars_info[i] != NULL) {
      ++alph_size;
    }
  }

  return alph_size;
}
