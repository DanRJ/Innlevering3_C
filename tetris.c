#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#define DELAY 100000
#define FIG_SIZE 3
#define HEIGHT 20
#define WIDTH 10
int checkKeyPressed(int ch, char *icon);
int **get2DArray(int cols, int rows);
int updateBoard();
void drawFigure(WINDOW* win, int **figure, int x, int y);
void drawBoard(WINDOW *win, int **board);
int main()
{
  WINDOW *tet_win;
  int ch, x, y, startx, starty, max_x, max_y;
  char icon = '0';
  
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

  x = 0;
  y = 0;

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
      drawBoard(tet_win, board);
      drawFigure(tet_win, figure, x, y);
      wrefresh(tet_win);
      usleep(DELAY);
      if(counter == 10)
      {
        ++y;
        counter = 0;
      }
      if(y > max_y) 
      {
        // If figure reaches bottom or touches another figure
        // Save figure position to board[][]
      }
      counter++;
    }
    else 
    {
      // The figure drawn rotates
      checkKeyPressed(ch, &icon);
    }
  }
  
  delwin(tet_win);

  endwin();

  return 0;
}

void drawFigure(WINDOW* win, int **figure, int x, int y)
{
  for(int i = 0; i < FIG_SIZE; i++)
  {
    for(int j = 0; j < FIG_SIZE; j++)
    {
      if(figure[i][j] == 1)
      {
        mvwaddch(win, y, j, '0');
      }
      printw("%d", figure[i][j]);
    }
    printw("\n");
  }
  printw("\n");
}

void drawBoard(WINDOW *win, int **board) 
{
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
      if(board[i][j] == 0)
      {
        mvwaddch(win, i, j, '.');
      }
      else 
      {
        mvwaddch(win, i, j, '0');
      }
    }
  }
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

int checkKeyPressed(int ch, char *icon) 
{
    switch(ch)
    {
      case KEY_LEFT:
        *icon = '1';
        break;
      case KEY_RIGHT:
        *icon = '2';
        break;
      case KEY_UP:
        *icon = '3';
        break;
      case KEY_DOWN:
        *icon = '4';
        break;
    }
    return 1;
}
