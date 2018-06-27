#include "my_string.h"
#include "filler.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>

void create_filler(filler_t *filler)
{
  filler->current_stream = NULL;
  filler->status = 0;
}

void destroy_filler(filler_t *filler)
{
  string_destroy(filler->current_stream);
}



void destroy_req(req_t *req)
{
  map_destroy(&req->map);
  map_destroy(&req->elem);
  free(req);
  req = NULL;
}

map_t map_init(int width, int height)
{
  map_t map;

  map.array = malloc(height * sizeof(char *));
  for(int i = 0; i < height; i++)
    map.array[i] = malloc((width + 1) * sizeof(char));
  map.h = height;
  map.w = width;

  return map;
}

map_t map_read(char *source, int pos, int w, int h)
{
  map_t map = map_init(w, h);

  for(int i = 0; i < map.h; i++)
  {
    for(int j = 0; j < map.w; j++)
    {
      map.array[i][j] = source[pos];
      pos++;
    }
    map.array[i][map.w] = '\0';
    pos++;
  }

  return map;
}

void map_destroy(map_t *map)
{
  for(int i = 0; i < map->h; i++)
    free(map->array[i]);
  free(map->array);
  map->array = NULL;
}

void printlog(const char *filename, const char *mode, const char *format, ...)
{
  va_list arg;
  FILE *logg;

  logg = fopen(filename, mode);
  va_start(arg, format);
  vfprintf(logg, format, arg);
  va_end(arg);

  fclose(logg);
}

int set_nonblocking(int fd)
{
  int flg;

  flg = fcntl(fd, F_GETFL, 0) | O_NONBLOCK;

  return fcntl(fd, F_SETFL, flg);
}

void fatal(char *msg)
{
  int size = strlen(msg);

  if(msg) write(2, msg, size);
  exit(1);
}