#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 4096
#define TEMPLATE_SIZE 17

typedef unsigned char uchar;

int read_template(uchar *template, FILE *in)
{
  int length = 0;
  uchar get = 0;
  while (fread(&get, sizeof(uchar), 1, in) == 1 && get != '\n') {
    template[length] = get;
    length += 1;
  }
  template[length] = '\0';
  return (length);
}

int get_buffer(int templateLen, uchar *symbols, uchar *buff, FILE *in)
{
  if (strlen((char *)symbols) != 0) {
    for (int i = 0; i < templateLen; i++) {
      buff[i] = symbols[i];
    }
  }
  for (int i = 0; i < BUFF_SIZE - TEMPLATE_SIZE; i++) {
    uchar chr = fgetc(in);
    if (!feof(in)) {
      buff[templateLen + i] = chr;
    } else {
      return 1;
    }
  }
  for (int i = BUFF_SIZE - TEMPLATE_SIZE; i < BUFF_SIZE - TEMPLATE_SIZE + templateLen; i++) {
    symbols[i - BUFF_SIZE + TEMPLATE_SIZE] = buff[i];
  }
  return 0;
}

void make_prefixes(uchar *str, int *prefixes)
{
  int i = 1;
  int j = 0;
  int len = strlen((const char *)str);
  while (i < len) {
    if (str[i] == str[j]) {
      prefixes[i] = j + 1;
      i++;
      j++;
    } else if (j == 0) {
      prefixes[i] = 0;
      i++;
    } else {
      j = prefixes[j - 1];
    }
  }
}

void kmp(FILE *in, int templateLen, const int *prefixes, const uchar *template,
         uchar *remainingSymbols, int64_t *globalIndex)
{
  uchar buffer[BUFF_SIZE];
  int index = templateLen;

  while (1) {
    int flag = get_buffer(templateLen, remainingSymbols, buffer, in);
    int buffer_len = strlen((char *)buffer + templateLen);

    while (index <= buffer_len) {
      int counter = 0;
      if (buffer[index + counter] == template[counter]) {
        while (counter < templateLen && buffer[index + counter] == template[counter]) {
          counter++;
        }
        printf("%" PRIu64 " %i ", (*globalIndex) + index - templateLen + 1, counter);
      }
      if (counter >= 1) {
        index += (counter - prefixes[counter - 1]);
      } else {
        index++;
      }
    }
    (*globalIndex) += BUFF_SIZE - TEMPLATE_SIZE;
    index = index - BUFF_SIZE + TEMPLATE_SIZE;
    memset(buffer, '\0', BUFF_SIZE);
    if (flag) {
      break;
      ;
    }
  }
}

int main()
{
  FILE *in = fopen("in.txt", "r");

  uchar template[TEMPLATE_SIZE];
  int template_len = read_template(template, in);
  if (template_len == 0) {
    fclose(in);
    return 0;
  }

  int *prefixes = calloc(template_len, sizeof(int));
  make_prefixes(template, prefixes);

  for (int i = 0; i < template_len; i++) {
    printf("%d ", prefixes[i]);
  }
  printf("\n");

  int64_t global_index = 0;
  uchar remaining_symbols[TEMPLATE_SIZE];

  kmp(in, template_len, prefixes, template, remaining_symbols, &global_index);

  fclose(in);
  free(prefixes);
  return 0;
}
