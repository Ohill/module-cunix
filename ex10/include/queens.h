#ifndef QUEENS_H_
#define QUEENS_H_

/*
 *  struct Cell
 *  figure = 'E' or 'Q'
 *  'E' means empty, 'Q' means queen
 */
typedef struct Cell {
  char figure;
} Cell;

/*
 *  Cell **table is two-dimensional array of Cell[size][size], where you should
 *  place your queens
 *  First queen is placed randomly, so you should put size - 1
 *  You should find first queen in array using search algorithm
 *  Example: If we have 8*8 chess table, you should add 7
 *  Adding means, that you set Cell.figure to 'Q'
 */
void place_queens(Cell **table, int size);
void backtrack(Cell **table, int n, int queen_row, int queen_col);
int check(Cell **table, int row, int col, int n);
void clear_row(Cell **table, int row, int col);
void change_queen(Cell **table, int row, int n);

#endif
