#include <stdlib.h>
#include <ctype.h>
#include "filler.h"
#include <stdio.h>
#include <string.h>

int find_size(char *dist, char *source, int start)
{
  int tmp = start;

  while(source[tmp] != '\n')
    tmp++;
  strncpy(dist, source + start, tmp - start);
  dist[tmp - start] = '\0';

  return tmp - 1;
}

pos_t parse_size(char *answer)
{
  int         i, size;
  char        *right, *left;
  pos_t       position;

  size = strlen(answer);
  left = malloc(size);
  right = malloc(size);
  memset(left, '\0', size);
  memset(right, '\0', size);
  for(i = 0; i < size && answer[i] != ' '; i++);
  left = strncpy(left, answer, i);
  right = strcpy(right, answer + i + 1);
  position.x = atoi(left);
  position.y = atoi(right);
  free(left);
  free(right);

  return position;
}

req_t          *parse_all(char *all)
{
  req_t        *req;
  pos_t        size;
  int          position = 2;
  char         read_str[32];

  req = (req_t *)malloc(sizeof(req_t));
  req->symbol = all[0];
  memset(read_str, '\0', 32);
  position = find_size(read_str, all, position) + 2;
  size = parse_size(read_str);
  req->map = map_read(all, position, size.y, size.x);
  position += req->map.h * (req->map.w + 1);
  position = find_size(read_str, all, position) + 2;
  size = parse_size(read_str);
  req->elem = map_read(all, position, size.y, size.x);

  return req;
}