#include "main.h"

#include <assert.h>
#include <stdlib.h>

#include "encoding.h"
#include "input.h"
#include "queue.h"
#include "tools.h"
#include "typedefs.h"
#include "utils.h"

/// input/output specification
// #define INPUT_FILE "input.bin"
#define INPUT_READ_MODE "r"
// #define OUTPUT_FILE "output.txt"
#define OUTPUT_WRITE_MODE "w"

void open_streams(FILE **input, FILE **output)
{
#ifdef INPUT_FILE
  *input = fopen(INPUT_FILE, INPUT_READ_MODE);
  assert(input != NULL);
#else
  *input = stdin;
#endif

#ifdef OUTPUT_FILE
  *output = fopen(OUTPUT_FILE, OUTPUT_WRITE_MODE);
  assert(output != NULL);
#else
  *output = stdout;
#endif
}

int main()
{
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  FILE *input = NULL;
  FILE *output = NULL;
  CharInfo *chars_info_dictionary[CHARSET_SIZE] = {NULL};
  CharInfo **chars_info_array = {NULL};
  CodingInfo codingInfo;
  size_t alphabet_size = 0;

  open_streams(&input, &output);

  scan_chars_frequencies_from_file(chars_info_dictionary, input);
  alphabet_size = get_alphabet_size(chars_info_dictionary);
  chars_info_array =
      get_chars_info_consistent(chars_info_dictionary, alphabet_size);
  assert(alphabet_size > 0);
  TreeNode *tree = build_huffman_tree(chars_info_array, alphabet_size);
  scan_codes_from_huffman_tree(tree, chars_info_dictionary);

#ifdef DEBUG
  print_codes_lexicographically(chars_info_array, alphabet_size, stdout);
  print_coding_stats(chars_info_array, alphabet_size, stdout);
#endif
  codingInfo = (CodingInfo){.huffman_tree = tree,
                            .alphabet_size = alphabet_size,
                            .chars_info_dictionary = chars_info_dictionary,
                            .chars_info_consistent = chars_info_array};
  archive_file_text_form(tree, &codingInfo, input, output);
#ifdef DEBUG
  archive_file_text_form(tree, &codingInfo, input, stdout);
#endif

  deallocate_chars_info(chars_info_array, alphabet_size);
  destroy_tree(tree);

  int rc = 0;
  rc = fclose(input);
  assert(rc != -1);
  rc = fclose(output);
  assert(rc != -1);

  return EXIT_SUCCESS;
}
