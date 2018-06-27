#include "filler.h"
#include "my_string.h"
#include <stdio.h>

int main()
{
  filler_t filler;

  create_filler(&filler);
  start_game(&filler);

  return 0;
}