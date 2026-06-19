#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define COLS 60
#define ROWS 30

int main() {
    // Hide cursor
    printf("\e[?25l");


    int quit = 0;
    int x[5], y[5];
    while (!quit)
    {
        // Render table
        printf("┌");
        for(int i = 0; i < COLS; i ++)
        printf("-");
        printf("┐\n");

        for (int j =0; j < ROWS; j++) {
        printf("|");
        for(int i = 0; i < COLS; i++) 
            printf("·");
        printf("|\n");
        }

        printf("└");
        for(int i = 0; i < COLS; i++)
        printf("-");
        printf("┘\n");
        
        // Move cursor back to top
        printf("\e[%iA", ROWS + 2);

        int gameover = 0;
        int xdir = 0, ydir = 1;
        int startx = ROWS / 2;
        int starty = 1;
        
        x[0] = startx;
        y[0] = starty;

        while(!quit && !gameover) {
            // Draw tetris
            printf("\e[%iB\e[%iC▓", 1, x[0]);
            printf("\e[%iF", 1);
            fflush(stdout);
            usleep(5 * 1000000 / 60);

            //Clear tetris
            // print
        }
       
    }
    // Show cursor
    printf("\e[?25h");
    return 0;
}