#include "filler.h"

strategy_t *strategy_init()
{
  strategy_t *strategy = (strategy_t*)malloc(sizeof(strategy_t));

  strategy->up = 0;
  strategy->down = 0;
  strategy->left = 0;
  strategy->right = 0;
  strategy->r_d_diag = 0;
  strategy->l_d_diag = 0;
  strategy->r_u_diag = 0;
  strategy->l_u_diag = 0;

  return strategy;
}

void destroy_strategy(strategy_t *tmp)
{
  free(tmp);
}