#ifndef TETRIS_H
#define TETRIS_H

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

/* 
 *  int checkRows(int **buffBoard, int **board);
 *    int **buffBoard  a 2D array
 *    int **board      a 2D array
 *
 *  returns            an int, (-1) if board is full, (1) if not
 */
int checkRows(int **buffBoard, int **board);

/*
 *  int stop(FILE *f, WINDOW *win, int **board, int **buffBoard, int **temp, int **figure);
 *    FILE   *f           a FILE, the FILEstream to be closed
 *    WINDOW *win         a WINDOW pointer, the window to be deleted
 *    int    **board      a 2D array, frees memory
 *    int    **buffBoard  a 2D array, frees memory
 *    int    **temp       a 2D array, frees memory
 *    int    **figure     a 2D array, frees memory
 *
 *  returns               an int
 */
int stop(FILE *f, WINDOW *win, int **board, int **buffBoard, int **temp, int **figure);

/*
 *  int checkKeyPressed(int ch,int **temp, int **figure, int *y, int *x, int max_y, int max_x, int *maxFigureY, int *maxFigureX);
 *    int ch            an int, the key pressed by the user
 *    int **temp        a 2D array, temporary holder for figure
 *    int **figure      a 2D array, the figure which is shown on screen
 *    int *y            an int pointer, points to the y-axis of the figure
 *    int *x            an int pointer, points to the x-axis of the figure
 *    int max_y         an int, maximal value of y
 *    int max_x         an int, maxmimal value of x
 *    int *maxFigureY   an int pointer, points to the max value of the figures y-axis
 *    int *maxFigureX   an int pointer, points to the max value of the figures x-axis
 *
 *  returns             an int, (1)
 */
int checkKeyPressed(int ch,int **temp, int **figure, int *y, int *x, int max_y, int max_x, int *maxFigureY, int *maxFigureX);

/*
 *  int **get2DArray(int cols, int rows);
 *    int cols    an int, number of columns in the 2D array
 *    int rows    an int, number of rows in the 2D array
 *
 *  returns       a memory-allocated 2D array
 */
int **get2DArray(int cols, int rows);

/*
 *  void drawFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);
 *    int **figure    a 2D array, the figure that should be drawn to the board
 *    int **board     a 2D array, the board which the figure should be drawn to
 *    int x           an int, x-axis of the figure
 *    int y           an int, y-axis of the figure
 *    int maxFigureY  an int, max y-axis value for the figure
 *    int maxFigureX  an int, max x-axis value for the figure
 *
 *  returns           (void)
 */
void drawFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);

/*
 *  void drawBoard(WINDOW *win, int **board);
 *    WINDOW *win     a WINDOW pointer, the window to be drawn on
 *    int    **board  a 2D array, the board which will be drawn on the window
 *
 *  returns           (void)
 */
void drawBoard(WINDOW *win, int **board);

/*
 *  void saveFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);
 *    int **figure    a 2D array, the figure that will be stored
 *    int **board     a 2D array, the temporary board to store the figure on
 *    int x           an int, the figures x-axis
 *    int y           an int, the figures y-axis
 *    int maxFigureY  an int, the figures max value for the y-axis
 *    int maxFigureX  an int, the figures max value for the x-axis
 *
 *  returns           (void)
 */
void saveFigure(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);

/*
 *  void saveBoardToFile(FILE* f, int **board);
 *    FILE *f       a FILE, the file the board should be saved on
 *    int  **board  a 2D array, the board which will be saved
 *
 *  returns         (void)
 */
void saveBoardToFile(FILE* f, int **board);

/*
 *  void renderBoardToScreen(int **board); 
 *    int **board   a 2D array, the board which will be rendered on screen
 *
 *  returns         (void)
 */
void renderBoardToScreen(int **board); 

/*  
 *  void renderFigureToScreen(int **figure);
 *    int **figure  a 2D array, the figure which will rendered onscreen
 *
 *  returns         (void)
 */
void renderFigureToScreen(int **figure);

/*
 *  int detectCollision(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);
 *    int **figure    a 2D array, the figure drawn on screen
 *    int **board     a 2D array, the temporary board
 *    int x           an int, the figures x-axis
 *    int y           an int, the figures y-axis
 *    int maxFigureY  an int, the max value for the figures y-axis
 *    int maxFigureX  an int, the max value for the figures x-axis
 *
 *  returns           (void)
 */
int detectCollision(int **figure, int **board, int x, int y, int maxFigureY, int maxFigureX);

/*
 *  void deleteLastFig(int **board, int lastX, int lastY);
 *    int **board   a 2D array, the board which is drawn unto the screen
 *    int lastX     an int, the previous x-axis of the figure
 *    int lastY     an int, the previous y-axis of the figure
 *
 *  returns         (void)
 */
void deleteLastFig(int **board, int lastX, int lastY);

/*
 *  void createRandomFigure(int **figure, int *maxFigureY, int *maxFigureX);
 *    int **figure      a 2D array, the 2D array to draw a figure on
 *    int *maxFigureY   an int pointer, the max value of the y-axis of the figure chosen
 *    int *maxFigureX   an int pointer, the max value of the x-axis of the figure chosen
 *
 *  returns             (void)
 */
void createRandomFigure(int **figure, int *maxFigureY, int *maxFigureX);

#endif
