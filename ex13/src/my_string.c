#include "my_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

stream_t *string_init()
{
  stream_t *tmp = (stream_t*)malloc(sizeof(stream_t));

  tmp->size = tmp->limit = 0;
  tmp->str = NULL;

  return tmp;
}

stream_t *string_create(char *str)
{
  int size, k;
  stream_t *tmp = string_init();

  size = strlen(str);
  tmp->size = size;
  k = size / BUF_SIZE + 1;
  tmp->limit = BUF_SIZE * k;
  tmp->str = (char*)malloc(sizeof(char) * k * BUF_SIZE);
  strcpy(tmp->str, str);

  return tmp;
}

void string_append(stream_t *ptr, char *str)
{
  int size = strlen(str), i;

  for(i = 0; i < size; i++)
    ptr->str[ptr->size+i] = str[i];
  ptr->size += size;
  ptr->str[ptr->size] = '\0';
}

void string_destroy(stream_t *str)
{
  free(str->str);
  free(str);
  str = NULL;
}
