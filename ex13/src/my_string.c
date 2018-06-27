#include "my_string.h"

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

void 			string_append(stream_t *s, char *str)
{
  	int 		len;

	len = strlen(str);
  	if(s->size + len > s->limit)
  	{
    		s->limit = s->limit * 2 + len;
    		s->str = realloc( s->str, s->limit * sizeof(char));
    		s->str[s->size + len] = 0;
  	}

    for(int i = 0; i < len; i++)
    		s->str[s->size + i] = str[i];
  	s->size += len;
  	s->str[s->size + len] = 0;
}

void string_destroy(stream_t *str)
{
  free(str->str);
  free(str);
}
