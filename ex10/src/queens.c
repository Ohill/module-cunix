#include "../include/queens.h"
#include <stdlib.h>
#include <stdio.h>

int check(Cell **table, int row, int col, int n)
{
	int i, xTemp, yTemp;
	for (i = 0; i < n; i++)
		if (table[row][i].figure == 'Q')
			return 1;
	for (i = 0; i < n; i++)
		if (table[i][col].figure == 'Q')
			return 1;
	xTemp = row, yTemp = col;
	while (xTemp < (n - 1) && yTemp < (n - 1))
	{
		xTemp++;
		yTemp++;
		if (table[xTemp][yTemp].figure == 'Q')
			return 1;
	}
	xTemp = row, yTemp = col;
	while (xTemp > 0 && yTemp > 0)
	{
		xTemp--;
		yTemp--;
		if (table[xTemp][yTemp].figure == 'Q')
			return 1;
	}
	xTemp = row, yTemp = col;
	while (xTemp < (n - 1) && yTemp > 0)
	{
		xTemp++;
		yTemp--;
		if (table[xTemp][yTemp].figure == 'Q')
			return 1;
	}
	xTemp = row, yTemp = col;
	while (xTemp > 0 && yTemp < (n - 1))
	{
		xTemp--;
		yTemp++;
		if (table[xTemp][yTemp].figure == 'Q')
			return 1;
	}
	return 0;
}

void backtrack(Cell **table, int n, int queen_row, int queen_col)
{
	int i, flg = 0;
	if (queen_row >= n) 
		return;
	for (i = queen_col; i < n; i++)
  {
	    if (!check(table, queen_row, i, n))
		  {
        flg = 1;
			  table[queen_row][i].figure = 'Q';
        clear_row(table, queen_row, i);
      }
      else if(table[queen_row][i].figure == 'Q')
      {
        flg = 1;
        break;
      }
  }
  if (!flg)
  {
    change_queen(table, queen_row - 1, n);
    backtrack(table, n, queen_row, 0);
  }
  backtrack(table, n, queen_row + 1, 0);
}

void change_queen(Cell **table, int row, int n)
{
  int i;
  for(i = 0; i < n; i++)
    if(table[row][i].figure == 'Q')
    {
      table[row][i].figure = 'E';
      break;
    }
  backtrack(table, n, row, i + 1);  
}

void clear_row(Cell **table, int row, int col)
{
  for(int i = 0; i < col; i++)
    table[row][i].figure = 'E';
} 

void place_queens(Cell **table, int size)
{
	backtrack(table, size, 0, 0);
}

