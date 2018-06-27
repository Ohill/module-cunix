#include "my_string.h"

int finish_of_read(stream_t* buf)
{
  unsigned int position = 0, elem_pos = 0;
  bool     frst_sz = true;
  char     arr_elem_size[32];
  pos_t    el_size;

  while(position <= buf->size)
  {
    if(buf->str[position] > '9' || buf->str[position] < '0')
      position++;
    else if(frst_sz)
    {
      while(position <= buf->size && buf->str[position] != '\n')
        position++;
      position++;
      frst_sz = false;
    }
    else
    {
      elem_pos = position;
      while(position <= buf->size && buf->str[position] != '\n')
        position++;
      if(buf->str[position] != '\n')
        return -1;
      else
      {
        find_size(arr_elem_size, buf->str, elem_pos);
        el_size = parse_size(arr_elem_size);
        if(buf->size <= position + el_size.x*(el_size.y + 1))
          return -1;
        else return 0;
      }
    }
  }
  return -1;
}

req_t *read_request(filler_t *filler)
{
  req_t *req;

  read_input(filler);
  if(finish_of_read(filler->current_stream) == 0)
    req = parse_all(filler->current_stream->str);

  return req;
}

void read_input(filler_t* filler)
{
  int read_num = 0;
  char read_str[BUF_SIZE];

  while(true)
  {
    memset(read_str, '\0', BUF_SIZE);
    read_num = read(0, read_str, BUF_SIZE-1);
    if(!filler->current_stream)
      filler->current_stream = string_create(read_str);
    else
      string_append(filler->current_stream, read_str);
    if(read_num <= 0)
      break;
  }

}
