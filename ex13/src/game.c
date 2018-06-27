#include "my_string.h"

pos_t play(strategy_t *strategy, req_t *core)
{
  FILE *logger;
  int flag = 0;
  strategy->result.x = -1;
  strategy->result.y = -1;
  pos_t res;

  res.x = -1;
  res.y = -1;
  logger = fopen("filler.log", "a");
  fprintf(logger, "Play\n");
  fprintf(logger, "core->map.h = %d, core->map.w = %d", core->map.h, core->map.w);
  fclose(logger);

  if(strategy->start == 0)
  {
    for(int i = 0; i < core->map.h; i++)
    {
      for(int j = 0; j < core->map.w; j++)
      {
        strategy->my_pos.y = i;
        strategy->my_pos.x = j;

        if(core->map.array[i][j] == core->symbol)
        {
          flag = 1;
          break;
        }
       if(flag)
         break;
      }
    }
    strategy->start = 1;
  }
  check_all(strategy);
      if(strategy->l_d_diag == 0)
      {
        for(int i = strategy->my_pos.y; i < core->map.h; i++)
          for(int j = strategy->my_pos.x; j >= 0; j--)
          {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
          }
      }

      if (strategy->r_u_diag == 0)
      {
        for(int i = strategy->my_pos.y; i < core->map.h; i--)
          for(int j = strategy->my_pos.x; j < core->map.w; j++)
          {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
          }
      }

      if (strategy->r_d_diag == 0)
      {
        for(int i = strategy->my_pos.y; i < core->map.h; i++)
          for(int j = strategy->my_pos.x; j < core->map.w ; j++)
          {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
          }
      }

      if (strategy->l_u_diag == 0)
      {
        for(int i = strategy->my_pos.y; i >= 0; i--)
          for(int j = strategy->my_pos.x; j >= 0; j--)
          {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
          }
      }

      if(strategy->right == 0)
      {
          int i = strategy->my_pos.y;
          for(int j = strategy->my_pos.x; j < core->map.w; j++)
          {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
          }
      }

      if(strategy->left == 0)
      {
          int i = strategy->my_pos.y;
          for(int j = strategy->my_pos.x; j >= 0; j--)
          {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
          }
      }
      
      if(strategy->up == 0)
      {
        int j = strategy->my_pos.x;
        for(int i  = strategy->my_pos.y; i >= 0; i--)
        {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
        }
      }

      if(strategy->down == 0)
      {
        int j = strategy->my_pos.x;
        for(int i  = strategy->my_pos.y; i <= core->map.h; i++)
        {
            res.x = j;
            res.y = i;
            if(!check_free_space(&(core->map), &(core->elem), res) && !check_connection(&(core->map), &(core->elem), res, core->symbol))
              return res;
        }
      }

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
  strategy->result.x = -1;
  strategy->result.y = -1;

  return strategy->result;
}

void check_all(strategy_t *strategy)
{
  srand(time(NULL));
  strategy->l_d_diag = rand() % 2;
  strategy->r_d_diag = rand() % 2;
  strategy->l_u_diag = rand() % 2;
  strategy->r_u_diag = rand() % 2;
  strategy->right = rand() % 2;
  strategy->left = rand() % 2;
  strategy->down = rand() % 2;
  strategy->up = rand() % 2;
}

bool check_in_first_sq(pos_t elem, int y_centr, int x_centr)
{
  return x_centr >= elem.x && y_centr >= elem.y ? true : false;
}

bool check_in_second_sq(pos_t elem, int y_centr, int x_centr)
{
  return x_centr >= elem.x && y_centr <= elem.y ? true : false;
}

bool check_in_third_sq(pos_t elem, int y_centr, int x_centr)
{
  return x_centr <= elem.x && y_centr >= elem.y ? true : false;
}

bool check_in_quad_sq(pos_t elem, int y_centr, int x_centr)
{
  return x_centr <= elem.x && y_centr <= elem.y ? true : false;
}

void start_game(filler_t *filler)
{
  req_t *req;
  pos_t position;
  fd_set rfds, wfds;
  struct timeval timeout;

  printlog("filler.log", "w", "Smart game\n");
  set_nonblocking(0);
  strategy_t *strategy = strategy_init();
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
        position = play(strategy, req);
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
  destroy_strategy(strategy);
}