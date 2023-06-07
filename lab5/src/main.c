#include "main.h"

#include <assert.h>
#include <stdlib.h>

#include "decoding.h"
#include "decoding_bitstring.h"
#include "encoding.h"
#include "encoding_bitstring.h"
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
  MODE_ENCODING_BITSTRING,
  MODE_DECODING_BITSTRING,
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

#ifdef DEBUG
  setbuf(*output, NULL);
#endif
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
  if (condingInfo == NULL) {
    return;
  }

  destroy_tree(condingInfo->huffman_tree);

  CharInfo **chars_info_array = condingInfo->chars_info_array;
  if (chars_info_array == NULL) {
    return;
  }

  for (int i = 0; i < condingInfo->alphabet_size; ++i) {
    free(chars_info_array[i]->huffman_code);
    free(chars_info_array[i]);
  }
  free(chars_info_array);
}

#define STOP_ON_TEST_N 5
void abort_on_test()
{
  int test_n = 0;
  FILE *filep = fopen("test/tmp_current_test.txt", "r+");
  assert(filep != NULL);
  setbuf(filep, NULL);

  int rc = fscanf(filep, "%d", &test_n);  // NOLINT
  assert(rc != 0);
  test_n += 1;

  if (test_n == STOP_ON_TEST_N) {
    rewind(filep);
    rc = fprintf(filep, "%d ", 0);
    assert(rc != 0);
    exit(3);
  } else {
    rewind(filep);
    rc = fprintf(filep, "%d ", test_n);
    assert(rc != 0);
  }

  rc = fclose(filep);
  assert(rc != EOF);
}

int main(int argc, char *argv[])
{
  // abort_on_test();
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
    // encoding_bitstring(&codingInfo, input, output);
    encoding(&codingInfo, input, output);
  }
  if (mode == MODE_DECODING) {
    // decoding_bitstring(&codingInfo, input, output);
    decoding(&codingInfo, input, output);
  }

  codinginfo_free(&codingInfo);

  close_streams_checked(input, output);
  return EXIT_SUCCESS;
}
