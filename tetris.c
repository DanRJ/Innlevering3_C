#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELAY 100000
#define FIG_SIZE 3
#define HEIGHT 20
#define WIDTH 10

int checkKeyPressed(int ch, int *y, int *x, int max_y, int max_x);
int **get2DArray(int cols, int rows);
int updateBoard();
void drawFigure(int **figure, int **board, int x, int y);
void drawBoard(WINDOW *win, int **board);
void saveFigure(int **figure, int **board, int x, int y);
void saveBoardToFile(FILE* f, int **board);
void renderBoardToScreen(int **board); 
void renderFigureToScreen(int **figure);
int detectCollision(int **figure, int **board, int *figurePos, int x, int y, int maxFigurePos);
void deleteLastFig(int **figure, int **board, int lastX, int lastY);

int main()
{
  WINDOW *tet_win;
  int ch, x, y, startx, starty, max_x, max_y, lastY, lastX;
  FILE *f = fopen("file.txt", "w");
  int figurePos[3];
  int maxFigurePos = 0;
  int **board;
  int **buffBoard;
  int **figure;
  figure = get2DArray(FIG_SIZE, FIG_SIZE);
  
  for(int i = 0; i < FIG_SIZE; i++) 
  {
    for(int j = 0; j < FIG_SIZE; j++) 
	  {
      if(i == 0) {
      figure[i][j] = 1;
     }
      if(figure[i][j] == 1)
      {
        figurePos[i] = j;
        if(maxFigurePos < j)
        {
          maxFigurePos = j;
        }
      }
	  }
  }
  board = get2DArray(HEIGHT, WIDTH);
  buffBoard = get2DArray(HEIGHT, WIDTH);
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
      buffBoard[i][j] = 0;
      board[i][j] = 0;
    }
  }

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
  curs_set(0);
  wrefresh(tet_win);
  refresh();
  getmaxyx(tet_win, max_y, max_x);
  int counter = 0;
  
  while(1) 
  {
    
    if((ch = getch()) == ERR) 
    {
      if(detectCollision(figure, buffBoard, figurePos, x , y, maxFigurePos) == 1)
      {
        printw("x: %d y: %d maxfig: %d", x, y, maxFigurePos);
        saveFigure(figure, buffBoard, x, y + maxFigurePos);
        //renderBoardToScreen(buffBoard);
        saveBoardToFile(f, buffBoard);
        y = 0;
      }
      // Draw a figure, move it downwards
      // until it hits some other figure
      deleteLastFig(figure, board, lastX, lastY);
      lastX = x;
      lastY = y;
      drawFigure(figure, board, x, y);
      drawBoard(tet_win, board);
      wrefresh(tet_win);
      
      usleep(DELAY);
      //if(counter == 10)
      //{
      
        counter = 0;
        y++;
      //}
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

int detectCollision(int **figure, int **board, int *figurePos, int x, int y, int maxFigurePos)
{
  if(y > 19 - maxFigurePos)
  {
    return 1;
  }
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
    }
  }
}
void deleteLastFig(int **figure, int **board, int lastX, int lastY) 
{
  if(lastY > -1) {
    int maxY = (FIG_SIZE + lastY);
    int maxX = (FIG_SIZE + lastX);
    if(maxY >= HEIGHT) maxY = HEIGHT;
  
    for(int i = lastY; i < maxY; i++)
    {
      for(int j = lastX; j < maxX; j++)
      { 
        board[i][j] = 0;
      }
    }
  }
}

void saveBoardToFile(FILE* f, int **board)
{
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++) 
    {
      fprintf(f, "%d", board[i][j]);
    }
    fprintf(f, "\n");
  }
  fprintf(f, "\n");
}

void renderFigureToScreen(int **figure)
{
  for(int i = 1; i <= FIG_SIZE; i++)
  {
    for(int j = 1; j <= FIG_SIZE; j++)
    {
      printw("%d", figure[i][j]);
    }
    printw("\n");
  }
  printw("\n");

}

void renderBoardToScreen(int **board) 
{
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
      printw("%d", board[i][j]);
    }
    printw("\n");
  }
  printw("\n");
}

void saveFigure(int **figure, int **board, int x, int y)
{
  int maxY = (FIG_SIZE + y);
  int maxX = (FIG_SIZE + x);
  if(maxY >= HEIGHT) maxY = HEIGHT;
  
  for(int i = y; i < maxY; i++)
  {
    for(int j = x; j < maxX; j++)
    {
      board[i][j] = figure[(i - y)][(j - x)];
    }
  }
}  

/*
*  drawFigure(int **figure, int **board, int x, int y)
*    int **figure     a 2D array which contains a certain figure
*    int **board      a 2D array which will draw the figure to screen
*    int x            an int which contains the y position for figure[1][1] on the board
*    int y            an int which contains the y position for figure[1][1] on the board
*  returns            void
*/
void drawFigure(int **figure, int **board, int x, int y)
{
  int maxY = (FIG_SIZE + y);
  int maxX = (FIG_SIZE + x);
  if(maxY >= HEIGHT) maxY = HEIGHT;
  if(maxX >= WIDTH) maxX = WIDTH;

  for(int i = y; i < maxY; i++)
  {
    for(int j = x; j < maxX; j++)
    {
      board[i][j] = figure[(i - y)][(j - x)];
    }
  }
}



/*
*  drawBoard(WINDOW *win, int **board, int **buffBoard, FILE *f)
*    WINDOW *win      a pointer to WINDOW, which it will write to
*    int **board      a 2D array which contains the static blocks in the board
*    int **buffBoard  a 2D array which contains the figure
*    FILE *f          a FILE which will write the board to file.txt
*  returns            void
*/
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
        mvwaddch(win, i, j, '#');
      }
    }
  }
}

/*
*  int **get2DArray(int rows, int cols)
*    int rows     number of rows for the 2D array
*    int cols     number of columns for the 2D array
*  returns        a 2D array with memory allocated
*/
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

/*
*  int checkKeyPressed(int ch, int *y, int *x, int max_y, int max_x)
*    int ch       the character pressed by the user
*    int *y       a pointer to the y position of the figure
*    int *x       a pointer to the x position of the figure
*    int max_y    an int that contains the max int for the y position
*    int max_x    an int that contains the max int for the x position
*  returns        an int which represents SUCCESS(1) or ERROR(0) 
*/
int checkKeyPressed(int ch, int *y, int *x, int max_y, int max_x) 
{
    switch(ch)
    {
      case KEY_LEFT:
        if(!(*x < 0 + FIG_SIZE))
          *x -= 1;
        break;
      case KEY_RIGHT:
        printw("MAX%d", max_x);
        if(!(*x > (max_x - FIG_SIZE) - 1))
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
