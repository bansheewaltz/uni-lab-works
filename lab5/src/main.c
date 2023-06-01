#include "main.h"

#include <assert.h>
#include <stdlib.h>

#include "decoding.h"
#include "deserialization.h"
#include "encoding.h"
#include "output.h"
#include "queue.h"
#include "tools.h"
#include "typedefs.h"
#include "utils.h"

#ifdef DEBUG
#include "debug.h"
#endif

/// input/output specification
// #define INPUT_FILE "input.bin"
#define INPUT_READ_MODE "r"
// #define OUTPUT_FILE "output.txt"
#define OUTPUT_WRITE_MODE "w"

typedef enum
{
  MODE_ENCODING,
  MODE_DECODING,
  MODE_UNDEFINED
} ProgramMode;

void open_streams_checked(FILE **input, FILE **output, int argc, char *argv[])
{
  if (argc > 1)
    *input = fopen(argv[1], INPUT_READ_MODE);
  else
    *input = stdin;
  assert(*input != NULL);

  if (argc > 2)
    *output = fopen(argv[2], OUTPUT_WRITE_MODE);
  else
    *output = stdout;
  assert(*output != NULL);
}

void close_streams_checked(FILE *input, FILE *output)
{
  int rc = 0;
  if (input != stdin) {
    rc = fclose(input);
    assert(rc != -1);
  }
  if (output != stdout) {
    rc = fclose(output);
    assert(rc != -1);
  }
}

ProgramMode read_operating_mode(FILE *input)
{
  int mode_parameter = getc(input);
  if (mode_parameter == 'c') {
    return MODE_ENCODING;
  }
  if (mode_parameter == 'd') {
    return MODE_DECODING;
  }
  return MODE_UNDEFINED;
}

void encoding(CodingInfo *codingInfo, FILE *input, FILE *output)
{
  scan_chars_frequencies_from_input(codingInfo->chars_info_dictionary, input);

  size_t alphabet_size = get_alphabet_size(codingInfo->chars_info_dictionary);
  assert(alphabet_size > 0);
  codingInfo->alphabet_size = alphabet_size;

  CharInfo **chars_info_dictionary = codingInfo->chars_info_dictionary;
  CharInfo **chars_info_array =
      get_chars_info_consistent(chars_info_dictionary, alphabet_size);
  TreeNode *tree = build_huffman_tree(chars_info_array, alphabet_size);

  codingInfo->huffman_tree = tree;
  codingInfo->chars_info_array = chars_info_array;

  scan_codes_from_huffman_tree(tree, chars_info_dictionary);
#ifdef DEBUG
  print_codes_lexicographically(chars_info_array, alphabet_size, stdout);
  print_coding_stats(chars_info_array, alphabet_size, stdout);
#endif
  encode_input_text_form(tree, codingInfo, input, output);
}

void decoding(CodingInfo *codingInfo, FILE *input, FILE *output)
{
  size_t file_size = (size_t)read_file_size_checked(input);
  assert(file_size > 0);

  size_t alphabet_size = (size_t)read_alphabet_size_checked(input);
  assert(alphabet_size > 0);

  TreeNode *tree = read_huffman_tree_text_form(alphabet_size, input);

  print_decoded_file_to_text(tree, file_size, input, output);
}

void codinginfo_initialize(CodingInfo *codingInfo)
{
  codingInfo->huffman_tree = NULL;
  codingInfo->alphabet_size = 0;
  codingInfo->chars_info_array = NULL;
  for (int i = 0; i < CHARSET_SIZE; ++i) {
    codingInfo->chars_info_dictionary[i] = NULL;
  }
}

void codinginfo_free(CodingInfo *condingInfo)
{
  CharInfo **chars_info_array = condingInfo->chars_info_array;
  destroy_tree(condingInfo->huffman_tree);
  for (int i = 0; i < condingInfo->alphabet_size; ++i) {
    free(chars_info_array[i]->huffman_code);
    free(chars_info_array[i]);
  }
  free(chars_info_array);
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  FILE *input = NULL;
  FILE *output = NULL;

  open_streams_checked(&input, &output, argc, argv);

  ProgramMode mode = read_operating_mode(input);
  if (mode == MODE_UNDEFINED) {
    puts("the operating mode is entered incorrectly or is not specified");
    close_streams_checked(input, output);
    return EXIT_FAILURE;
  }

  CodingInfo codingInfo;
  codinginfo_initialize(&codingInfo);

  if (mode == MODE_ENCODING) {
    encoding(&codingInfo, input, output);
  }
  if (mode == MODE_DECODING) {
    decoding(&codingInfo, input, output);
  }

  codinginfo_free(&codingInfo);

  close_streams_checked(input, output);
  return EXIT_SUCCESS;
}
