#include "my_string.h"

void create_filler(filler_t *filler)
{
  filler->current_stream = NULL;
  filler->status = 0;
}

void destroy_filler(filler_t *filler)
{
  string_destroy(filler->current_stream);
}

int check_free_space(map_t *map, map_t *new_elem, pos_t p)
{
  for(int i = 0; i < new_elem->h; i++)
    for(int j = 0; j < new_elem->w; j++)
      if(new_elem->array[i][j] == '*')
      {
        if(i + p.y < map->h && j + p.x < map->w && i + p.y >= 0 && j + p.x >= 0)
        {
          if(map->array[i + p.y][j + p.x] != '.')
            return -1;
        }
        else
         return -1;
      }
  return 0;
}

int check_connection(map_t *map, map_t *new_elem, pos_t pos, char symbol)
{
  for(int i = 0; i < new_elem->h; i++)
    for(int j = 0; j < new_elem->w; j++)
      if(new_elem->array[i][j] != '.')
      {
        if(i+pos.y+1 < map->h && map->array[i+pos.y+1][j+pos.x] == symbol)
            return 0;
        if (i+pos.y-1 >= 0 && map->array[i+pos.y-1][j+pos.x] == symbol)
            return 0;
        if (j+pos.x+1 < map->w && map->array[i+pos.y][j+pos.x+1] == symbol)
            return 0;
        if (j+pos.x-1 >= 0 && map->array[i+pos.y][j+pos.x-1] == symbol)
            return 0;
      }

  return -1;
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