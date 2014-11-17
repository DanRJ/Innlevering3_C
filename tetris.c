#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#define DELAY 30000
#define FIG_SIZE 2

int checkKeyPressed(int ch, char *icon);
int **getFigure();
int updateBoard();
int main()
{
  WINDOW *tet_win;
  int ch, height, width, x, y, startx, starty, max_x, max_y;
  char icon = '0';
  height = 20;
  width = 10;
  
  int area[height][width];

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
  
  tet_win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
  box(tet_win, 0, 0);

  wrefresh(tet_win);
  refresh();
  getmaxyx(tet_win, max_y, max_x);
  int counter = 0;
  
  while(1) 
  {
    
    if((ch = getch()) == ERR) 
    {
      mvwaddch(tet_win, y, x, icon);
      wrefresh(tet_win);
      usleep(DELAY);
      if(counter == 10)
      {
        ++y;
        counter = 0;
      }
      if(y > max_y) 
      {
        y = 0;
        wclear(tet_win);
        box(tet_win, 0, 0);
      }
      counter++;
    }
    else 
    {
      checkKeyPressed(ch, &icon);
    }
  }
  
  delwin(tet_win);

  endwin();

  return 0;
}

int **getFigure() 
{
  int* val = calloc(FIG_SIZE * FIG_SIZE, sizeof(int));
  int** figure = malloc(FIG_SIZE * sizeof(int));
  for(int i = 0; i < FIG_SIZE; ++i) 
  {
    figure[i] = val + i * FIG_SIZE;
  }
  return figure;
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
