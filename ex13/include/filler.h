#ifndef _FILLER_H_
#define _FILLER_H_

#include "libs.h"

typedef struct  map_s
{
  int           h;
  int           w;
  char          **array;
}               map_t;


typedef struct  pos_s
{
  int           x;
  int           y;
}               pos_t;

typedef struct  req_s
{
  char          symbol;
  map_t         map;
  map_t        elem;
}               req_t;

typedef struct  stream_s
{
  char          *str;
  unsigned int  size;
  unsigned int  limit;
}               stream_t;

typedef struct  filler_s
{
  stream_t      *current_stream;
  int           status;
  int           find_enemy;
}               filler_t;

typedef struct  strategy_s
{
  pos_t my_pos;
  pos_t enemy_pos;
  int up;
  int down;
  int left;
  int right;
  int r_d_diag;
  int l_d_diag;
  int r_u_diag;
  int l_u_diag;
  int start;
}               strategy_t;

/*Functions for reading*/
req_t *read_request(filler_t *filler);
void read_input(filler_t* filler);

/*Functions for strategy */
strategy_t *strategy_init();
void destroy_strategy(strategy_t *tmp);
bool check_in_quad_sq(pos_t elem, int y_centr, int x_centr);
bool check_in_third_sq(pos_t elem, int y_centr, int x_centr);
bool check_in_second_sq(pos_t elem, int y_centr, int x_centr);
bool check_in_first_sq(pos_t elem, int y_centr, int x_centr);
void check_all(strategy_t *strategy);


/*Functions for parsing*/
req_t *parse_all(char *all);
pos_t parse_size(char *answer);
int find_size(char *dist, char *source, int start);

map_t map_init(int width, int height);
map_t map_read(char *source, int pos, int w, int h);
void map_destroy(map_t *map);

/*Functions for game logic*/
void start_game(filler_t *filler);
pos_t play(strategy_t *strategy, req_t *core);

/*Functions for printing*/
void print_pos(pos_t p);

/*Tools*/
int check_free_space(map_t *map, map_t *new_elem, pos_t p);
int check_connection(map_t *map, map_t *new_elem, pos_t pos, char symbol);
int set_nonblocking(int fd);
void fatal(char *msg);
void сreate_filler(filler_t *filler);
void destroy_filler(filler_t *filler);
void destroy_req(req_t *req);

#endif // _FILLER_H_
