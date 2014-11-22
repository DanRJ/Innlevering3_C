Innlevering 3

The assignment is not 100% done, but this was a tricky assignment
and I may have overcomplicated it a little bit.

I found it hard to process the fact that an array starts at [0]
but the window starts at 1, and how they interact, which leads to
interesting "features" in my program.

Features such as not properly updating a screen after a row has
been completed, which will lead to some interesting behaviour.

I would rename my tetris game to ImpossibruTetrisru.

The game closes when you reach the top, that was a choice I made.

There are few comments in the tetris.c, but all the functions has been
commented on in the header file (tetris.h).

To run my program you need to write this in the terminal:

gcc tetris.c client.c -std=gnu99 -lncurses

The -std=gnu99 flag is needed to get access to the usleep() function.

There are also a couple of debugging functions in the program, namely
renderFigureToScreen, renderBoardToScreen and saveBoardToFile.

I decided to leave them in there, so I could continue on playing with
this program in the future.
