#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// input/output specification
#define INPUT_FILE "input.bin"
#define INPUT_READ_MODE "rb"
#define OUTPUT_FILE "output.txt"
#define OUTPUT_WRITE_MODE "w"
// character set & memory limits
#define EIGHT_BIT_CHARACTER_SET_SIZE 256
#define CHARSET_SIZE EIGHT_BIT_CHARACTER_SET_SIZE
#define BUFFER_SIZE 4096
// radix sort options
#define COUNTING_SORT_BASE 10
#define DESCENDING true
#define ASCENDING false
// huffman tree implementation features
#define MAX_TREE_HEIGHT EIGHT_BIT_CHARACTER_SET_SIZE
#define INTERNAL_NODE_SYMBOL 'x'
// huffman tree prefix code bits
#define LEFT_CHILD 0
#define RIGHT_CHILD 1
// huffman tree serialization bits
#define INTERNAL_NODE 0
#define LEAF_NODE 1
// bit-wise writing options
#define FLUSH_BYTE true
#define CONTINUE false
// bit-wise printing options
#define NEW_LINE true
#define SAME_LINE false

typedef unsigned char uchar;
typedef uint8_t byte;

typedef struct CharInfo CharInfo;
struct CharInfo {
  uchar character;
  size_t freq;
  uint8_t *huffman_code;
  size_t code_len;
};

typedef struct TreeNode TreeNode;
struct TreeNode {
  uchar character;
  size_t freq;
  TreeNode *left, *right;
};

typedef struct CodingInfo CodingInfo;
struct CodingInfo {
  TreeNode *huffman_tree;
  CharInfo **chars_info_dictionary;
  CharInfo **chars_info_consistent;
  size_t alphabet_size;
};

typedef struct Queue Queue;
struct Queue {
  int front, rear;
  int capacity;
  TreeNode **array;
};

void check_null_pointer(void *p)
{
  if (p == NULL) {
    perror("archiver error");
    exit(1);
  }
}

TreeNode *create_tree_node(uchar character, size_t freq)
{
  TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
  check_null_pointer(temp);

  temp->left = temp->right = NULL;
  temp->character = character;
  temp->freq = freq;

  return temp;
}

Queue *create_queue(int capacity)
{
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  check_null_pointer(queue);

  queue->front = queue->rear = -1;
  queue->capacity = capacity;
  queue->array = (TreeNode **)malloc(queue->capacity * sizeof(TreeNode *));

  return queue;
}

bool is_queue_size_one(Queue *queue)
{
  return queue->front == queue->rear && queue->front != -1;
}

bool is_queue_empty(Queue *queue)
{
  return queue->front == -1;
}

bool is_queue_full(Queue *queue)
{
  return queue->rear == queue->capacity - 1;
}

void enqueue(Queue *queue, TreeNode *item)
{
  assert(!is_queue_full(queue));

  queue->array[++queue->rear] = item;
  if (queue->front == -1) {
    ++queue->front;
  }
}

TreeNode *dequeue(Queue *queue)
{
  if (is_queue_empty(queue)) {
    return NULL;
  }

  TreeNode *temp = queue->array[queue->front];
  if (queue->front == queue->rear) {
    queue->front = queue->rear = -1;
  } else {
    ++queue->front;
  }

  return temp;
}

void reverse_queue(Queue *queue)
{
  if (queue->front == -1) {
    return;
  }

  TreeNode *item = dequeue(queue);
  reverse_queue(queue);
  enqueue(queue, item);
}

TreeNode *get_queue_front(Queue *queue)
{
  if (is_queue_empty(queue)) {
    return NULL;
  }

  return queue->array[queue->front];
}

TreeNode *find_queue_min(Queue *firstQueue, Queue *secondQueue)
{
  if (is_queue_empty(firstQueue)) {
    return dequeue(secondQueue);
  }
  if (is_queue_empty(secondQueue)) {
    return dequeue(firstQueue);
  }

  if (get_queue_front(firstQueue)->freq < get_queue_front(secondQueue)->freq) {
    return dequeue(firstQueue);
  }
  if (get_queue_front(firstQueue)->freq ==
      get_queue_front(secondQueue)->freq) {
    return dequeue(firstQueue);
  }
  return dequeue(secondQueue);
}

#ifdef DEBUG
void display_queue(Queue *queue)
{
  if (queue->rear == -1) {
    puts("Queue is empty");
    return;
  }

  for (int i = queue->front; i <= queue->rear; i++) {
    printf("%d ", queue->array[i]->character);
  }

  printf("\n");
}
#endif

void destroy_queue(Queue *queue)
{
  free(queue->array);
  free(queue);
}

void counting_sort_nodes(TreeNode **array, size_t const array_size,
                         size_t const place, bool const order)
{
  assert(array_size > 0);
  int max_digit = COUNTING_SORT_BASE - 1;
  int count[COUNTING_SORT_BASE] = {0};
  TreeNode **array_output =
      (TreeNode **)malloc(sizeof(TreeNode *) * array_size);
  // for (size_t i = 0; i < array_size; ++i){
  //   array_output[i] = malloc()
  // }

  // Calculate count of elements
  for (size_t i = 0; i < array_size; ++i) {
    count[(array[i]->freq / place) % 10]++;
  }
  // Calculate cumulative count
  if (order == DESCENDING) {
    for (int i = max_digit; i > 0; --i) {
      count[i - 1] += count[i];
    }
  } else {
    for (int i = 1; i < max_digit + 1; ++i) {
      count[i] += count[i - 1];
    }
  }
  // Place the elements in sorted order
  for (size_t i = array_size - 1; i < array_size; --i) {
    array_output[--count[(array[i]->freq / place) % 10]] = array[i];
  }
  // Copy the sorted elements into original array
  for (size_t i = 0; i < array_size; ++i) {
    array[i] = array_output[i];
  }
  free(array_output);
}

void radix_sort_nodes(TreeNode **array, size_t const array_size,
                      bool const order)
{
  size_t max = array[0]->freq;

  for (size_t i = 1; i < array_size; ++i) {
    if (array[i]->freq > max) {
      max = array[i]->freq;
    }
  }

  for (int place = 1; max / place > 0; place *= 10) {
    counting_sort_nodes(array, array_size, place, order);
  }
}

void sort_queue(Queue *queue, size_t size, bool order)
{
  radix_sort_nodes(queue->array, size, order);
}

TreeNode *build_huffman_tree(CharInfo **chars_info_array, size_t alphabet_size)
{
  if (alphabet_size == 1) {
    return create_tree_node(chars_info_array[0]->character,
                            chars_info_array[0]->freq);
  }

  TreeNode *left, *right, *top;
  Queue *firstQueue = create_queue(alphabet_size);
  Queue *secondQueue = create_queue(alphabet_size);

  for (size_t i = 0; i < alphabet_size; ++i) {
    enqueue(firstQueue, create_tree_node(chars_info_array[i]->character,
                                         chars_info_array[i]->freq));
  }

#ifdef DEBUG
  puts("original queue:");
  display_queue(firstQueue);
#endif
  sort_queue(firstQueue, alphabet_size, DESCENDING);
#ifdef DEBUG
  puts("sorted queue:");
  display_queue(firstQueue);
#endif
  reverse_queue(firstQueue);
#ifdef DEBUG
  puts("reversed queue:");
  display_queue(firstQueue);
#endif

  while (!(is_queue_empty(firstQueue) && is_queue_size_one(secondQueue))) {
    right = find_queue_min(firstQueue, secondQueue);
    left = find_queue_min(firstQueue, secondQueue);

    top = create_tree_node(INTERNAL_NODE_SYMBOL, left->freq + right->freq);
    top->left = left;
    top->right = right;
    enqueue(secondQueue, top);
  }

  TreeNode *huffman_tree = dequeue(secondQueue);
  destroy_queue(firstQueue);
  destroy_queue(secondQueue);

  return huffman_tree;
}

bool is_node_leaf(TreeNode *node)
{
  return !(node->left) && !(node->right);
}

void destroy_tree(struct TreeNode *root)
{
  if (root == NULL) {
    return;
  }

  destroy_tree(root->left);
  destroy_tree(root->right);

  free(root);
  return;
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

void scan_chars_frequencies_from_file(CharInfo **chars_info, FILE *const input)
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

TreeNode *make_new_queue_node(char character, int freq)
{
  TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
  check_null_pointer(temp);

  temp->left = temp->right = NULL;
  temp->character = character;
  temp->freq = freq;

  return temp;
}

uint8_t *array_uint8_duplicate(uint8_t src[], size_t len)
{
  uint8_t *dst = (uint8_t *)malloc(sizeof(uint8_t) * len);

  for (size_t i = 0; i < len; ++i) {
    dst[i] = src[i];
  }

  return dst;
}

void print_code(CharInfo *charInfo, FILE *const output)
{
  fprintf(output, "%d : ", charInfo->character);
  for (size_t i = 0; i < charInfo->code_len; ++i) {
    fprintf(output, "%" PRIu8, charInfo->huffman_code[i]);
  }
  fprintf(output, "\n");
}

void print_codes_lexicographically(CharInfo **chars_info_array,
                                   size_t alphabet_size, FILE *output)
{
  for (size_t i = 0; i < alphabet_size; ++i) {
    print_code(chars_info_array[i], output);
  }
}

void print_coding_stats(CharInfo **chars_info_array, size_t alphabet_size,
                        FILE *output)
{
  int file_size = 0;
  int code_length = 0;
  double information_capacity = 0;

  for (size_t i = 0; i < alphabet_size; ++i) {
    file_size += chars_info_array[i]->freq;
    code_length += chars_info_array[i]->code_len * chars_info_array[i]->freq;
  }
  for (size_t i = 0; i < alphabet_size; ++i) {
    information_capacity +=
        (double)chars_info_array[i]->freq / file_size *
        log2((double)file_size / chars_info_array[i]->freq);
  }
  double redundancy =
      1.0 - (double)file_size / code_length * information_capacity;

  fprintf(output, "Information capacity = %.6f\n", information_capacity);
  fprintf(output, "Input file size = %d\n", file_size);
  fprintf(output, "Code length = %d\n", code_length);
  fprintf(output, "Redundancy = %.6f\n", redundancy);
}

void open_files(FILE **input, FILE **output)
{
  *input = fopen(INPUT_FILE, INPUT_READ_MODE);
  *output = fopen(OUTPUT_FILE, OUTPUT_WRITE_MODE);

  check_null_pointer(*input);
  check_null_pointer(*output);
}

void print_byte_in_binary(uchar byte, bool formatting, FILE *output)
{
  for (int i = CHAR_BIT - 1; i >= 0; --i) {
    fprintf(output, "%d", (byte >> i) & 1 ? 1 : 0);
  }
  if (formatting == NEW_LINE) {
    fprintf(output, "\n");
  }
}

void print_char_in_binary(uchar character, bool formatting, FILE *output)
{
  print_byte_in_binary(character, formatting, output);
}

void print_bits(void const *const ptr, size_t const size, FILE *output)
{
  uint8_t *b = (uint8_t *)ptr;
  uint8_t byte;

  for (int j, i = size - 1; i >= 0; --i) {
    for (j = CHAR_BIT - 1; j >= 0; --j) {
      byte = (b[i] >> j) & 0b00000001;
      fprintf(output, "%" PRIu8, byte);
    }
  }
}

void array_int_print(int array[], int len, FILE *output)
{
  for (int i = 0; i < len; ++i) {
    fprintf(output, "%d", array[i]);
  }
  fprintf(output, "\n");
}

void array_char_print(uchar array[], int len, FILE *output)
{
  for (int i = 0; i < len; ++i) {
    fprintf(output, "%d ", array[i]);
  }
  fprintf(output, "\n");
}

uchar bit_to_char(int bit)
{
  if (bit == 0) {
    return '0';
  }
  return '1';
}

void preorder_traversal_printing(TreeNode *root, FILE *output,
                                 size_t *bits_printed)
{
  if (root == NULL) {
    return;
  }

  if (!is_node_leaf(root)) {
    putc(bit_to_char(INTERNAL_NODE), output);
    *bits_printed += 1;
    preorder_traversal_printing(root->left, output, bits_printed);
    preorder_traversal_printing(root->right, output, bits_printed);
  }
  if (is_node_leaf(root)) {
    putc(bit_to_char(LEAF_NODE), output);
    *bits_printed += 1;
    print_char_in_binary(root->character, SAME_LINE, output);
    *bits_printed += 8;
  }
}

size_t print_huffman_tree_to_text(TreeNode *tree, FILE *output)
{
  if (tree == NULL) {
    return 0;
  }
  size_t bits_printed = 0;

  preorder_traversal_printing(tree, output, &bits_printed);

  return bits_printed;
}

void set_bit(uchar *byte, bool bit, int position)
{
  *byte |= bit << (CHAR_BIT - position);
}

void reset_byte(uchar *byte, int *bit_position)
{
  *byte = 0b00000000;
  *bit_position = 1;
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

void serialize_char(uchar character, FILE *output)
{
  for (int i = CHAR_BIT - 1; i >= 0; --i) {
    writebit_buffered((character >> i) & 1 ? 1 : 0, CONTINUE, output);
  }
}

void preorder_traversal_serialization(TreeNode *root, FILE *output)
{
  if (root == NULL) {
    return;
  }

  if (!is_node_leaf(root)) {
    writebit_buffered(INTERNAL_NODE, CONTINUE, output);
    preorder_traversal_serialization(root->left, output);
    preorder_traversal_serialization(root->right, output);
  }
  if (is_node_leaf(root)) {
    writebit_buffered(LEAF_NODE, CONTINUE, output);
    serialize_char(root->character, output);
  }
}

void serialize_huffman_tree(TreeNode *tree, size_t alph_size, FILE *output)
{
  if (tree == NULL) {
    return;
  }

  uchar alph_size_as_byte = (uchar)alph_size;
  fwrite(&alph_size_as_byte, sizeof(uchar), 1, output);
  preorder_traversal_serialization(tree, output);
  writebit_buffered(0, FLUSH_BYTE, output);
}

bool get_bit(uchar byte, int position)
{
  return (byte >> (CHAR_BIT - position)) & 0b00000001;
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

uchar deserialize_char(FILE *input)
{
  int bit_position = 1;  // in byte, from left
  byte character = 0b00000000;

  for (; bit_position <= CHAR_BIT; ++bit_position) {
    set_bit(&character, readbit_buffered(input), bit_position);
  }

  return character;
}

void preorder_traversal_deserialization(TreeNode *root, int alph_size,
                                        FILE *input, uchar *characters_array,
                                        int depth)
{
  if (root == NULL) {
    return;
  }
  static int recovered_characters_count = 0;
  if (recovered_characters_count == alph_size) {
    return;
  }

  uint8_t bit = readbit_buffered(input);
  if (bit == INTERNAL_NODE) {
    TreeNode *left = (TreeNode *)malloc(sizeof(TreeNode));
    TreeNode *right = (TreeNode *)malloc(sizeof(TreeNode));

    root->left = left;
    root->right = right;
    preorder_traversal_deserialization(left, alph_size, input,
                                       characters_array, depth + 1);
    preorder_traversal_deserialization(right, alph_size, input,
                                       characters_array, depth + 1);
  }
  if (bit == LEAF_NODE) {
    root->left = NULL;
    root->right = NULL;
    root->character = deserialize_char(input);
    characters_array[recovered_characters_count] = root->character;
    ++recovered_characters_count;
#ifdef DEBUG
    printf("%d\n", root->character);
#endif
  }
  if (bit != INTERNAL_NODE && bit != LEAF_NODE) {
    exit(1);
  }
}

TreeNode *deserialize_huffman_tree(int alph_size, FILE *input,
                                   uchar **characters_array)
{
  TreeNode *tree = (TreeNode *)malloc(sizeof(TreeNode));
  *characters_array = (uchar *)malloc(sizeof(uchar) * alph_size);

  preorder_traversal_deserialization(tree, alph_size, input, *characters_array,
                                     0);

  return tree;
}

int read_alphabet_size(FILE *const input)
{
  byte alph_size;
  fread(&alph_size, sizeof(uchar), 1, input);

  if (alph_size == 0b00000000) {
    return EIGHT_BIT_CHARACTER_SET_SIZE;
  }
  return (int)alph_size;
}

void preoder_traversal_codes_scan(TreeNode *root, CharInfo **chars_info,
                                  uint8_t code[], size_t depth)
{
  if (root->left) {
    code[depth] = LEFT_CHILD;
    preoder_traversal_codes_scan(root->left, chars_info, code, depth + 1);
  }
  if (root->right) {
    code[depth] = RIGHT_CHILD;
    preoder_traversal_codes_scan(root->right, chars_info, code, depth + 1);
  }

  if (is_node_leaf(root)) {
    chars_info[root->character]->huffman_code =
        array_uint8_duplicate(code, depth);
    chars_info[root->character]->code_len = depth;
  }
}

void scan_codes_from_huffman_tree(TreeNode *root, CharInfo **chars_info)
{
  uint8_t code[MAX_TREE_HEIGHT];
  size_t depth = 0;

  preoder_traversal_codes_scan(root, chars_info, code, depth);
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

// void print_array_uint8(uint8_t *array, size_t array_size, FILE *output) {
//   for (size_t i = 0; i < array_size; ++i) {
//     fprintf(output, "%" PRIu8, array[i]);
//   }
// }
void print_code_array(CharInfo *charInfo, FILE *output)
{
  for (size_t i = 0; i < charInfo->code_len; ++i) {
    fprintf(output, "%" PRIu8, charInfo->huffman_code[i]);
  }
}

size_t print_encoded_file_to_text(CharInfo **chars_info_dictionary,
                                  FILE *input, FILE *output)
{
  rewind(input);
  uchar buffer[BUFFER_SIZE];
  size_t chars_read;
  size_t bits_printed = 0;

  while ((chars_read = fread(buffer, sizeof(uchar), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < chars_read; ++i) {
      print_code_array(chars_info_dictionary[buffer[i]], output);
      bits_printed += chars_info_dictionary[buffer[i]]->code_len;
    }
  }

  return bits_printed;
}

void fill_last_byte(size_t *bits_printed, FILE *output)
{
  if (*bits_printed % 8 != 0) {
    for (; *bits_printed % 8 != 0; ++*bits_printed) {
      putc('0', output);
    }
  }
}

void archive_file_text_form(TreeNode *tree, CodingInfo *codingInfo,
                            FILE *input, FILE *output)
{
  size_t bits_printed = 0;
  uint8_t alphabet_size = (uint8_t)codingInfo->alphabet_size;
  uint32_t input_file_length = 0;

  for (size_t i = 0; i < codingInfo->alphabet_size; ++i) {
    input_file_length += codingInfo->chars_info_consistent[i]->freq;
  }

  print_bits(&input_file_length, sizeof(uint32_t), output);
  bits_printed += CHAR_BIT * sizeof(uint32_t);
#ifdef DEBUG
  fprintf(output, "\n");
#endif
  print_bits(&alphabet_size, sizeof(uint8_t), output);
  bits_printed += CHAR_BIT * sizeof(uint8_t);
#ifdef DEBUG
  fprintf(output, "\n");
#endif
  bits_printed = print_huffman_tree_to_text(tree, output);
  fill_last_byte(&bits_printed, output);
#ifdef DEBUG
  fprintf(output, "\n");
#endif
  bits_printed = print_encoded_file_to_text(codingInfo->chars_info_dictionary,
                                            input, output);
  fill_last_byte(&bits_printed, output);
  (void)bits_printed;
}

void deallocate_chars_info(CharInfo **chars_info_array, size_t alphabet_size)
{
  for (size_t i = 0; i < alphabet_size; ++i) {
    free(chars_info_array[i]->huffman_code);
    free(chars_info_array[i]);
  }
  free(chars_info_array);
}

CharInfo **get_chars_info_consistent(CharInfo **const chars_info_dictionary,
                                     size_t alphabet_size)
{
  CharInfo **chars_info_array =
      (CharInfo **)malloc(sizeof(CharInfo *) * alphabet_size);

  for (size_t i = 0, j = 0; j < alphabet_size; ++i) {
    if (chars_info_dictionary[i] != NULL) {
      chars_info_array[j++] = chars_info_dictionary[i];
    }
  }

  return chars_info_array;
}

int main()
{
#ifdef DEBUG
  setbuf(stdout, NULL);
#endif
  FILE *input = NULL, *output = NULL;
  CharInfo *chars_info_dictionary[CHARSET_SIZE] = {NULL};
  CharInfo **chars_info_array = {NULL};
  CodingInfo codingInfo;
  size_t alphabet_size = 0;

  open_files(&input, &output);
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
  fclose(input);
  fclose(output);

  return EXIT_SUCCESS;
}
