#include <ncurses.h>

int main()
{
  WINDOW *tet_win;
  int ch, height, width, x, y, startx, starty;

  height = 20;
  width = 10;
  
  int area[height][width];

  startx = 0;
  starty = 0;
  
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  
  printw("Press F1 to exit");
  refresh();
  
  tet_win = newwin(height, width, starty, startx);
  keypad(tet_win, TRUE);
  box(tet_win, 0, 0);
  wrefresh(tet_win);
  refresh();
  while((ch = wgetch(tet_win)) != KEY_F(1)) 
  {
    switch(ch)
    {
      case KEY_LEFT:
        break;
      case KEY_RIGHT:
        break;
      case KEY_UP:
        break;
      case KEY_DOWN:
        break;
    }
  }
  delwin(local_win);

  endwin();

  return 0;
}
