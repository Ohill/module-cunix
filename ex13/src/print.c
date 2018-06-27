#include "filler.h"
#include <stdio.h>

void print_pos(pos_t pos)
{
  dprintf(1, "%i %i", pos.x, pos.y);
}