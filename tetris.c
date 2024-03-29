#include "tetris.h"
int checkRows(int **buffBoard, int **board)
{
  int row = -1;
  for(int i = 0; i < HEIGHT; i++)
  {
    int countRow = 0;
    for(int j = 0;  j < WIDTH; j++)
    {
      if(buffBoard[i][j] == 1) {
        countRow++;
      }
      if(board[1][j] == 1)
      {
        return -1;
      }
    }
    if(countRow == 10)
    {
      row = i;
      
      for(int k = 0; k < WIDTH; k++)
      {
        buffBoard[row][k] = 0;
        board[row][k] = 0;
      }
      for(int m = 0; m < HEIGHT - 1; m++)
      {
        for(int n = 0; n < WIDTH; n++)
        {
          board[m + 1][n] = board[m][n];
          buffBoard[m + 1][n] = board[m][n];
        }
      }
    }
  }
  return 1;
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
          break;
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

void drawFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX)
{
  int maxY = (maxFigureY + y) + 1;
  int maxX = (maxFigureX + x) + 1;
  if(maxY > HEIGHT) maxY = HEIGHT;
  if(maxX > WIDTH) maxX = WIDTH;
  
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

int counter = 0;
int checkKeyPressed(int ch, int **temp, int **figure, int *y, int *x, int max_y, int max_x, int *maxFigureY, int *maxFigureX) 
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
