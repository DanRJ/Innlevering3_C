#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#define DELAY 100000
#define FIG_SIZE 3
#define HEIGHT 20
#define WIDTH 10

int checkKeyPressed(int ch, int *y, int *x, int max_y, int max_x);
int **get2DArray(int cols, int rows);
int updateBoard();
void drawFigure(WINDOW* win, int **figure, int **board, int x, int y);
void drawBoard(WINDOW *win, int **board, int **buffBoard, FILE *f);

int main()
{
  WINDOW *tet_win;
  int ch, x, y, startx, starty, max_x, max_y;
  char icon = '0';
  FILE *f = fopen("file.txt", "w");
  int **board;
  int **buffBoard;
  int **figure;
  figure = get2DArray(FIG_SIZE, FIG_SIZE);
  
  for(int i = 0; i < FIG_SIZE; i++) 
  {
    for(int j = 0; j < FIG_SIZE; j++) 
	  {
      figure[i][j] = 0;
      if(i == 1)
      {
        figure[i][j] = 1;
      }
	  }
  }
  board = get2DArray(HEIGHT, WIDTH);
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
      board[i][j] = 0;
    }
  }
  buffBoard = board;
  startx = 0;
  starty = 0;
  
  max_x = 0;
  max_y = 0;

  x = 1;
  y = 1;

  initscr();
  cbreak();
  noecho();
  // Choose to not have a keypad for tetris window
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  
  tet_win = newwin(HEIGHT, WIDTH, (LINES - HEIGHT) / 2, (COLS - WIDTH) / 2);

  wrefresh(tet_win);
  refresh();
  getmaxyx(tet_win, max_y, max_x);
  int counter = 0;
  
  while(1) 
  {
    
    if((ch = getch()) == ERR) 
    {
      // Draw a figure, move it downwards
      // until it hits some other figure
      drawFigure(tet_win, figure, buffBoard, x, y);
      drawBoard(tet_win, board, buffBoard, f);
      wrefresh(tet_win);
      sleep(1);
      //if(counter == 10)
      //{
        ++y;
        //counter = 0;
      //}
      if(y >= max_y) 
      {
        // If figure reaches bottom or touches another figure
        // Save figure position to board[][]
	y = 1;
      }
      counter++;
    }
    else 
    {
      // The figure drawn rotates
      checkKeyPressed(ch, &y, &x, max_y, max_x);
    }
  }
  fclose(f);
  delwin(tet_win);

  endwin();

  return 0;
}

void drawFigure(WINDOW* win, int **figure, int **board, int x, int y)
{
  for(int i = y - 1; i < (FIG_SIZE + y) - 1; i++)
  {
    for(int j = x - 1; j < (FIG_SIZE + x) - 1; j++)
    {
      board[i][j] = figure[(i - y) + 1][(j - x) + 1];
    }
  }
}

void drawBoard(WINDOW *win, int **board, int **buffBoard, FILE *f) 
{
  
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
      fprintf(f, "%d", buffBoard[i][j]);
      if(board[i][j] == 0)
      {
        mvwaddch(win, i, j, '.');
      }
      else 
      {
        mvwaddch(win, i, j, '0');
      }
    }
fprintf(f, "\n");
  }

fprintf(f, "\n");

}

int **get2DArray(int rows, int cols) 
{
  int **arr;
  arr = (int**)malloc(rows * sizeof(int *));
  for(int i = 0; i < rows; ++i) 
  {
    arr[i] = (int*) malloc(cols * sizeof(int));
  }
  return arr;
}

int checkKeyPressed(int ch, int *y, int *x, int max_y, int max_x) 
{
    switch(ch)
    {
      case KEY_LEFT:
        *x -= 1;
        break;
      case KEY_RIGHT:
        *x += 1;
        break;
      case KEY_UP:
        //rotate figure
        break;
      case KEY_DOWN:
        *y += 1;
        break;
    }
    return 1;
}
