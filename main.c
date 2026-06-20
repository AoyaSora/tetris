#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define COLS 50
#define ROWS 30

int main() {
    // Hide cursor
    // printf("\e[?25l");


    int quit = 0;
    int tetris[5][5];
    int board[ROWS][COLS] = {0};
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
        int startx = (COLS -1) / 2 + 1;
        int starty = 3;
        int tetY = starty, tetX = startx; // it's base on EscapeSequene
        // top left "·" potition
        printf("\e[%i;%iH❤",starty,2); 

        while(!quit && !gameover) {
            // Clear tetris
            if(tetY!=starty) printf("\e[%i;%iH·",tetY-1, tetX);

            // Draw tetris
            printf("\e[%i;%iH#",tetY, tetX);
            if(tetY == 2 + ROWS || board[tetY-2][tetX-2] == 1) {
                board[tetY-3][tetX-2] = 1;
                tetY = starty; tetX = startx;
            }
            tetY++;

            fflush(stdout);
            usleep(5 * 1000000 / 60);
        }
       
    }
    // Show cursor
    printf("\e[?25h");
    return 0;
}