/*******************************/
/* Ashley Krattiger            */
/* 2/24/18                     */
/* Solves Sudoku puzzles       */
/*******************************/

/*
 * Let's solve sudoku problems!
 *
 * see http://norvig.com/sudoku.html
 */
#include <stdio.h>
#include <ctype.h>

#define BLANK '.'

/*****************************************
  * board[][]: holds current puzzle
*****************************************/
char board[9][9] = {{'0'}};


/***********************************************************************
  * inputs: int x, int y, char c
  * int x: x position of square being checked (column)
  * int y: y position of square being checked (row)
  * char c: character being checked as valid input for square
  * int r: row position
  * int c1: column position
  * int br: box row, used to check a specific 9x9 box
  * int bc: box column, used to check a specific 9x9 box
  * int row: used to iterate through specific box
  * int col: used to iterate through specific box
  * determines if c at index is valid, given the character array, grid
  * returns 1 if valid, 0 if not 
***********************************************************************/
int valid(int x, int y, char c)
{
  int r, cl, br, bc, row, col;
  if(c == BLANK) return 1;
  if(x > 8 || y > 8) return 0;
  if(board[y][x] != BLANK && board[y][x] != c) return 0;
  for(r = 0; r < 9; r++) if(r != y && board[r][x] == c) return 0;
  for(cl = 0; cl < 9; cl++) if(cl != x && board[y][cl] == c) return 0;

  if(x < 3) bc = 0;
  else if(x < 6) bc = 3;
  else bc = 6;

  if(y < 3) br = 0;
  else if(y < 6) br = 3;
  else br = 6;
  
  for(row = br; row < br+3; row++)
    {
      for(col = bc; col < bc+3; col++)
	{
	  if((row != y && col != x) || (row != y && col == x) ||
	     (row == y && col != x))
	    {
	      if(board[row][col] == c) return 0;
	    }
	}
    }
  
  return 1;
}

/******************************************
 * int r: used to iterate through rows
 * int c: used to iterate through columns
 * determines if the given grid is valid
 * returns 1 if valid, 0 if not
******************************************/
int validate()
{
  int r,c;
  for(r = 0; r < 9; r++)
    {
      for(c = 0; c < 9; c++)
	{
	  if(valid(c, r, board[r][c]) == 0) return 0;
	}
    }
  return 1;
}


/***************************************************************************
 * Inputs: int x, int y, int i
 * int x: x position of box to be solved (column)
 * int y: y position of box to be solved (row)
 * int i: represents which value in list of valid values will be returned
 * int v: incremented value to be compared to i
 * char c: number to be checked as possible solution (accounting for ASCII)
 * Use a search algorithm to find all possible values
 * Returns i-th valid value
***************************************************************************/
char search(int x, int y, int i)
{
  int v = 0;
  char c = 49;
  while(c < 58)
    {
      if(valid(x, y, c))
	{
	  v++;
	  if(v == i) return c;
	}
      c++;
    }
  return BLANK;
}

/******************************************************
 * int r: represents row
 * int c: represents column
 * Prints out the grid array to screen in a 9x9 square
 * Returns nothing
******************************************************/
void prettyPrint()
{
  int r,c;
  for(r = 0; r < 9; r++)
    {
      if(r == 3 || r == 6) printf("------+-------+------\n");
      for(c = 0; c < 9; c++)
	{
	  if(c == 3 || c == 6) printf("| ");
	  if(c == 8) printf("%c\n", board[r][c]);
          else printf("%c ", board[r][c]);
	}
    }
}

/*************************************************************************
 * int r: represents row
 * int c: represents column
 * int err: keeps track if there was an error found
 * char in: iterates through input
 * Reads in one puzzle and checks for errors
 * Returns 0 if there is an error with length or input, 1 if there are no
 * errors, 2 if the file ended early, 3 if the line was fine but the file
 * ended after, and 4 if the EOF was the only thing on the line
*************************************************************************/
int readLine()
{
  int r = 0,c = 0,err = 1;
  char in = 0;
  
  while(in != '\n' && in != EOF)
    {
      in = getchar();
      if(in == EOF) return 4;
      if(r < 9)
        {
          if(in == '\n') err = 0;
          else if(in == EOF) err = 2;
          else if(in != BLANK && !isdigit(in)) err = 0;
          else board[r][c] = in;
        }
      if(in != EOF) printf("%c", in);
      if(c == 8)
        {
          c = 0;
          r++;
        }
      else c++;
    }
  if(err != 1) return err;
  else if(r > 9 || (r == 9 && c > 1)) return 0;
  else if(in == EOF) return 3;
  else return 1;
}

/*****************************************************
 * Inputs: int x, int y
 * int x: x location of square to be checked (column)
 * int y: y location of square to be checked (row)
 * int i: used to iterate through possibilities
 * int nextX: x value to be input in next recursion
 * int nextY: y value to be input in next recursion
 * int checkSol: hold value returned by recursion
 * Solves the sudoku puzzle
 * returns 1 if there is a solution, 0 if no solution
*****************************************************/
int findSolution(int x, int y)
{
  int i;
  int nextX = x;
  int nextY = y;
  int checkSol;

  if(x == 8)
    {
      nextX = 0;
      nextY++;
    }
  else nextX++;  

  if(board[y][x] == BLANK)
    {

      for(i = 1; i <= 9; i++)
	{
	  board[y][x] = search(x, y, i);
	  if(board[y][x] == BLANK) return 0;
	  if(x == 8 && y == 8) return 1;
	  checkSol = findSolution(nextX, nextY);
	  if(checkSol == 1) return 1;
	  else if(checkSol == 0) board[y][x] = BLANK;
	}
      return 0;
    }
  else
    {
      if(x == 8 && y == 8) return 1;
      else
        {
          checkSol = findSolution(nextX, nextY);
          if(checkSol == 1) return 1;
          else return 0;
        }
    }
}

/***************************************************************
 * int result: holds return of readLine
 * int check: holds return of validate
 * Runs through input and prints error messages or the solution
***************************************************************/
int main()
{
  int result = 1;
  int check;
  while(result < 2)
    {
      result = readLine();
      if(result == 0 || result == 2) printf("Error\n\n");
      else if(result != 4)
        {
          check = validate();
          if(check == 0) printf("Error\n\n");
          else
            {
              check = findSolution(0,0);
              if(check == 0) printf("No solution\n\n");
              else
                {
                  printf("solution:\n");
                  prettyPrint();
                }
            }
        }
    }
  return 0;
}


