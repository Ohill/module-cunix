#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "filler.h"
#include "my_string.h"

int check_free_space(map_t *map, map_t *new_elem, pos_t p)
{
  for(int i = 0; i < new_elem->h; i++)
    for(int j = 0; j < new_elem->w; j++)
      if(new_elem->array[i][j] == '*')
      {
        if(i+p.y < map->h && j+p.x < map->w && i+p.y >= 0 && j+p.x >= 0)
        {
          if(map->array[i+p.y][j+p.x] != '.')
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

pos_t play(req_t *core)
{
  pos_t res;
  FILE *logger;
  res.x = -1;
  res.y = -1;

  logger = fopen("filler.log", "a");
  fprintf(logger, "Play\n");
  fprintf(logger, "core->map.h = %d, core->map.w = %d", core->map.h, core->map.w);
  fclose(logger);
  for(int i = 0; i < core->map.h; i++)
    for(int j = 0; j < core->map.w; j++)
    {
      logger = fopen("filler.log", "a");
      fprintf(logger, "Play circle\n");
      fclose(logger);
      res.x = j;
      res.y = i;
      if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
      {
        logger = fopen("filler.log", "a");
        fprintf(logger, "RES POS = %d %d\n", res.x, res.y);
        fclose(logger);

        return res;
      }
    }
  res.x = -1;
  res.y = -1;

  return res;
}

void start_game(filler_t *filler)
{
  req_t *req;
  pos_t position;
  fd_set rfds, wfds;
  struct timeval timeout;

  printlog("filler.log", "w", "Smart game\n");
  set_nonblocking(0);
  while(1)
  {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    switch(filler->status)
    {
      case 0: FD_SET(0, &rfds); break;
      case 1: FD_SET(1, &wfds); break;
    }
    printlog("filler.log", "a", "Status: %s\n", filler->status ? "writing" : "reading");
    timeout.tv_usec = 0;
    timeout.tv_sec = 1;
    select(2, &rfds, &wfds, NULL, &timeout);
    if(FD_ISSET(0, &rfds))
    {
      req = read_request(filler);
      if(req) {
        position = play(req);
        filler->status = 1;
      }
    }
    if(FD_ISSET(1, &wfds))
    {
      printlog("filler.log", "a", "Position: %i %i\n", position.x, position.y);
      print_pos(position);
      string_destroy(filler->current_stream);
      filler->current_stream = NULL;
      destroy_req(req);
      req = NULL;
      filler->status = 0;
    }
  }
}