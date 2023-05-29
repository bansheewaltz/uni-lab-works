#include <stddef.h>
#include <stdlib.h>

#include "main.h"

int calculate_alphabet_size(int *freq_dictionary)
{
  int alph_size = 0;

  for (int i = 0; i < EIGHT_BIT_CHARACTER_SET_SIZE; ++i) {
    if (freq_dictionary[i] != 0) {
      ++alph_size;
    }
  }

  return alph_size;
}

CharInfo *create_and_initialize_char_info(uchar character)
{
  CharInfo *charInfo = (CharInfo *)malloc(sizeof(CharInfo));

  charInfo->character = character;
  charInfo->freq = 0;
  charInfo->huffman_code = NULL;
  charInfo->code_len = 0;

  return charInfo;
}

CharInfo **get_chars_info_consistent(CharInfo **chars_info_dictionary,
                                     size_t alphabet_size)
{
  CharInfo **chars_info_array =
      (CharInfo **)malloc(sizeof(CharInfo *) * alphabet_size);
  if (chars_info_array == NULL) {
    return NULL;
  }

  for (size_t i = 0, j = 0; j < alphabet_size; ++i) {
    if (chars_info_dictionary[i] != NULL) {
      chars_info_array[j++] = chars_info_dictionary[i];
    }
  }

  return chars_info_array;
}
