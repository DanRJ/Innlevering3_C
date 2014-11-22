#include "tetris.h"
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
  
  // Populate the boards with 0s
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
  // Chose to not have a keypad for tetris window
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
        int ret = checkRows(buffBoard, board);
        if(ret == -1)
        {
          stop(f, tet_win, board, buffBoard, temp, figure);
          return 0;
        }
        //renderFigureToScreen(figure);
        //renderBoardToScreen(buffBoard);
        //saveBoardToFile(f, buffBoard);
        createRandomFigure(figure, &maxFigureY, &maxFigureX);
        y = 0;
        x = (WIDTH / 2) - 1;
      }
      deleteLastFig(board, lastX, lastY);
      lastX = x;
      lastY = y;
      
      drawFigure(figure, board, x, y, maxFigureY, maxFigureX);
      
      // Populates the new data in buffBoard to board
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
      checkKeyPressed(ch,temp, figure, &y, &x, max_y, max_x, &maxFigureY, &maxFigureX);
    }
  }
  stop(f, tet_win, board, buffBoard, temp, figure);
  
  return 0;
}
