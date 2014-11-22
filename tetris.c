#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DELAY 100000
#define FIG_SIZE 3
#define HEIGHT 20
#define WIDTH 10

int checkRows(int **board);
int stop(FILE *f, WINDOW *win, int **board, int **buffBoard, int **temp, int **figure);
int checkKeyPressed(int ch,int **temp, int **figure, int **board, int *y, int *x, int max_y, int max_x, int *maxFigureY, int *maxFigureX);
int **get2DArray(int cols, int rows);
int updateBoard();
void drawFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);
void drawBoard(WINDOW *win, int **board);
void saveFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);
void saveBoardToFile(FILE* f, int **board);
void renderBoardToScreen(int **board); 
void renderFigureToScreen(int **figure);
int detectCollision(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);
void deleteLastFig(int **board, int lastX, int lastY);
void createRandomFigure(int **figure, int *maxFigureY, int *maxFigureX);

int main()
{
  WINDOW *tet_win;
  int ch, x, y, max_x, max_y, lastY, lastX;
  FILE *f = fopen("file.txt", "w");
  int maxFigureY = 0;
  int maxFigureX = 0;
  int **board;
  int **buffBoard;
  int **figure;
  int **temp;
  
  board = NULL;
  buffBoard = NULL;
  figure = NULL;
  temp = NULL;

  temp = get2DArray(FIG_SIZE, FIG_SIZE);
  figure = get2DArray(FIG_SIZE, FIG_SIZE);
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
  
  max_x = 0;
  max_y = 0;
  
  x = (WIDTH / 2) - 1;
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
  max_y = max_y - 1;
  max_x = max_x - 1;
  createRandomFigure(figure, &maxFigureY, &maxFigureX);

  while(1) 
  {
    if((ch = getch()) == ERR) 
    {
      if(detectCollision(figure, buffBoard, x , y, maxFigureY, maxFigureX) == 1)
      {
        saveFigure(figure, buffBoard, x, y, maxFigureY, maxFigureX);
        if(checkRows(buffBoard) == -1)
        {
          stop(f, tet_win, board, buffBoard, temp, figure);
          return 0;
        }
        //renderFigureToScreen(figure);
        //renderBoardToScreen(buffBoard);
        saveBoardToFile(f, buffBoard);
        createRandomFigure(figure, &maxFigureY, &maxFigureX);
        y = 0;
        x = (WIDTH / 2) - 1;
      }
      deleteLastFig(board, lastX, lastY);
      lastX = x;
      lastY = y;
      drawFigure(figure, board, x, y, maxFigureY, maxFigureX);
      for(int k = 0; k < HEIGHT; k++)
      {
        for(int l = 0; l < WIDTH; l++)
        {
          if(buffBoard[k][l] == 1)
          {
            board[k][l] = 1;
          }
        }
      }
      drawBoard(tet_win, board);

      wrefresh(tet_win); 
      usleep(DELAY);
      y++;
    }
    else 
    {
      // The figure drawn rotates
      checkKeyPressed(ch,temp, figure, buffBoard, &y, &x, max_y, max_x, &maxFigureY, &maxFigureX);
    }
  }
  stop(f, tet_win, board, buffBoard, temp, figure);
  return 0;
}
int checkRows(int **board)
{
  for(int i = 0; i < HEIGHT; i++)
  {
    for(int j = 0;  j < WIDTH; j++)
    {
      if(board[1][j] == 1)
      {
        return -1;
      }
    }
  }
}
int stop(FILE *f, WINDOW *win, int **board, int **buffBoard, int **temp, int **figure)
{
  free(board);
  free(buffBoard);
  free(temp);
  free(figure);
  fclose(f);
  delwin(win);
  endwin();
}
int detectCollision(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX)
{
  if((y + maxFigureY) + 1 == HEIGHT)
  {
    return 1;
  }
  for(int i = 0; i < FIG_SIZE; i++)
  {
    for(int j = 0; j < FIG_SIZE; j++)
    {
      if(figure[i][j] == 1)
      {
        // Check for y + i + 1
        // Check for x + j + 1
        if(board[y + i][(x + j)] == 1)
        {
          return 1;
        }
        if(board[(y + i) + 1][(x + j)] == 1)
        {
          return 1;
        }
      }
    }
  }
  return 0;
}

void createRandomFigure(int **figure, int *maxFigureY, int *maxFigureX)
{
  srand(time(NULL));
  int ran = rand() % 6 + 1;
  
  for(int k = 0; k < FIG_SIZE; k++)
  {
    for(int l = 0; l < FIG_SIZE; l++)
    {
      figure[k][l] = 0;
    }
  }
  for(int i = 0; i < FIG_SIZE; i++)
  {
    for(int j = 0; j < FIG_SIZE; j++)
    {
      switch(ran)
      {
        case 1:
          if(i == 1) {
            figure[i][j] = 1;
          }
          break;
        case 2:
          if(i == 0) {
            figure[i][j] = 1;
          }
          figure[1][1] = 1;
          break;
        case 3:
          if(j == 1) {
            figure[i][j] = 1;
          }
          figure[0][0] = 1;
          figure[2][2] = 1;
          break;
        case 4:
          if(i == 1) {
            figure[i][j] = 1;
          }
          if(j == 1) {
            figure[i][j] = 1;
          }
          break;
        case 5:
          figure[i][j] = 1;
          break;
        case 6:
          if(j == 1) {
            figure[i][j] = 1;
          }
      }
      if(figure[i][j] == 1)
      {
        if(*maxFigureY < i)
        {
          *maxFigureY = i;
        }
        if(*maxFigureX < j)
        {
          *maxFigureX = j;
        }
      }
    }
  }

}
void deleteLastFig(int **board, int lastX, int lastY) 
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
  for(int i = 0; i < FIG_SIZE; i++)
  {
    for(int j = 0; j < FIG_SIZE; j++)
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

void saveFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX)
{
  drawFigure(figure, board, x, y, maxFigureY, maxFigureX);
}  

/*
*  drawFigure(int **figure, int **board, int x, int y)
*    int **figure     a 2D array which contains a certain figure
*    int **board      a 2D array which will draw the figure to screen
*    int x            an int which contains the y position for figure[1][1] on the board
*    int y            an int which contains the y position for figure[1][1] on the board
*  returns            void
*/
void drawFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX)
{
  int maxY = (maxFigureY + y) + 1;
  int maxX = (maxFigureX + x) + 1;
  if(maxY > HEIGHT) maxY = HEIGHT;
  if(maxX > WIDTH) maxX = WIDTH;
 // printw("x: %d y: %d maxY: %d maxX: %d\n", x, y, maxY, maxX);
  for(int i = y; i < maxY; i++)
  {
    for(int j = x; j < maxX; j++)
    {
      if(figure[i - y][j - x] == 1)
      {
        board[i][j] = figure[i - y][j - x];
      }
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
int counter = 0;
int checkKeyPressed(int ch, int **temp, int **figure, int **board, int *y, int *x, int max_y, int max_x, int *maxFigureY, int *maxFigureX) 
{
  switch(ch)
  {
    case KEY_LEFT:
      if(!(*x < 1))
        *x -= 1;
      break;
    case KEY_RIGHT:
      // Check for wall
      if(!(*x >= (max_x - *maxFigureX)))
        *x += 1;
      break;
    case KEY_UP:
      counter++;
      for(int i = 0; i < FIG_SIZE; i++)
      {
        for(int j = 0; j < FIG_SIZE; j++)
        {
          temp[i][j] = figure[2 - j][i];
        }
      }
      for(int k = 0; k < FIG_SIZE; k++)
      {
        for(int l = 0; l < FIG_SIZE; l++)
        {
          figure[k][l] = temp[k][l];
          if(figure[k][l] == 1)
          {
            if(counter % 4 == 0) 
            {
              *maxFigureY = 0;
            }
            if(*maxFigureY < k)
            {
              *maxFigureY = k;
            }
            if(*maxFigureX < l)
            {
              *maxFigureX = l;
            }
          }
        }
      }
      break;
    case KEY_DOWN:
      *y += 1;
      break;
  }
  return 1;
}
